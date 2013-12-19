<?php
/**
 * Created by JetBrains PhpStorm.
 * User: walter
 * Date: 19/12/13
 * Time: 15.30
 * To change this template use File | Settings | File Templates.
 */
function wimtvpro_embedded() {
    global $base_url;
    $code = explode("embedded/", $_GET['q']);

    if (count($code)>1) {

        $codeArray = explode("/", $code[1]);
        $streamItem = $codeArray[1];
        $jSonST = wimtvpro_detail_showtime(true, $streamItem);
        $arrayjSonST = json_decode($jSonST);

        $arrayST["showtimeIdentifier"] = $arrayjSonST->{"showtimeIdentifier"};
        $arrayST["title"] = $arrayjSonST->{"title"};
        $arrayST["duration"] = $arrayjSonST->{"duration"};
        $arrayST["categories"] = $arrayjSonST->{"categories"};
        $arrayST["description"] = $arrayjSonST->{"description"};
        $arrayST["thumbnailUrl"] = $arrayjSonST->{"thumbnailUrl"};
        $arrayST["contentId"] = $arrayjSonST->{"contentId"};
        $arrayST["url"] = $arrayjSonST->{"url"};
        $ch = curl_init();
        if (variable_get('nameSkin')!="") {
            $directory = file_create_url('public://skinWim');
            $skin = "&skin=" . $directory . "/" . variable_get('nameSkin') . ".zip";
        }
        else
            $skin = "&skin=" . $base_url . "/" . drupal_get_path('module', 'wimtvpro') . "/skin/default.zip";

        $params = "get=1&width=" . variable_get('widthPreview') . "&height=" . variable_get('heightPreview') . $skin;
        //$params = "get=1&width=500px&height=280px" . $skin;
        $response = apiGetPlayerShowtime($arrayST["contentId"], $params);

        $args = array('response' => $response,
                      'title' => $arrayST["title"],
                      'description' => $arrayST["description"],
                      'duration' => $arrayST["duration"],
                      'categories' => $arrayST["categories"]);

        echo render_template('embedded/video.php', $args);
    }

}

function wimtvpro_embeddedAll() {
    $urlEmbedded = variable_get("urlEmbeddedPlayerWimtv");
    $replaceContent = variable_get('replaceContentWimtv');
    //echo $_GET['q'];
    $code = explode("embeddedAll/", $_GET['q']);
    //echo $code;
    //echo $code[1];
    if (count($code)>1) {
        $codeArra = explode("/", $code[1]);
        $contentItem = $codeArra[0];
        $result = db_query("SELECT * FROM {wimtvpro_videos} WHERE contentidentifier='" . $contentItem . "'");
        $video = $result->fetchAll();
        if (variable_get('nameSkin')!="") {
            $directory = file_create_url('public://skinWim');
        }
        else
            $directory = base_path() . drupal_get_path('module', 'wimtvpro');

        $output = "";
        $dimensions = "width:" . variable_get('widthPreview') . ",height:" . variable_get('heightPreview');
        $dimension = " width='" . (variable_get('widthPreview')+50) . "px' height='" . (variable_get('heightPreview')+50)  . "px'";
        //$dirJwPlayer = drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";

        $dimensioneFissaW = variable_get('widthPreview');
        $dimensioneFissaH = variable_get('heightPreview') + 100;
        $output .= "<div style='text-align:center;width:" . $dimensioneFissaW . "px; height:" . $dimensioneFissaH .  "px'>";
        $output .= "<div id='container'></div>";

        $dimension = " width='" . (variable_get('widthPreview')+50) . "px' height='" . (variable_get('heightPreview')+50)  . "px'";
        $configFile  = wimtvpro_viever_jwplayer ($_SERVER['HTTP_USER_AGENT'],$video[0]->contentidentifier,$video);

        $output .= "<script type='text/javascript'>jwplayer('container').setup({";
        if (variable_get('nameSkin')!="") $output .= "skin: '" . $directory . "/" . variable_get('nameSkin') . ".zip',";
        else $output .= "skin: '" . base_path() . drupal_get_path('module', 'wimtvpro') . "/skin/default.zip',";
        $embeddedEncode = str_replace("+", " " , urlencode("<iframe src='" . $GLOBALS['base_url'] . "/wimtvpro/viewEmbeddedPlayer/" . $video[0]->contentidentifier . "'" . $dimension . "></iframe>"));
        if (!isset($video[0]->urlThumbs)) $thumbs[1] = "";
        else $thumbs = explode ('"',$video[0]->urlThumbs);

        $urlPlay = explode("$$",$video[0]->urlPlay);
        $output .= $dimensions . "," . $configFile;

        $output .= "'plugins': {

			   'sharing-3': {

				   'code': '" . $embeddedEncode   . "'
			   }},";

        $output .= "image: '" . $thumbs[1] . "',});</script>&nbsp;";

        $output .= "<h3>" . $video[0]->title . " (Preview)</h3>";
        $output .= "<p>Duration: <b>" . $video[0]->duration . "</b>";
        if (count($video[0]->categories)>0) {
            $output .= "<br/>Categories<br/>";
            foreach ($video[0]->categories as $key => $value) {
                $valuescCatST = "<i>" . $value->categoryName . ":</i> ";
                $output .= $valuescCatST;
                foreach ($value->subCategories as $key => $value) {
                    $output .= $value->categoryName . ", ";
                }
                $output = drupal_substr($output, 0, -2);
                $output .= "<br/>";
            }
            $output .= "</p>";
        }


        $output .= "</div>";

    }

    echo $output;

}

function wimtvpro_plus_embeddedAll() {
    $urlEmbedded = variable_get("urlEmbeddedPlayerWimtv");
    $replaceContent = variable_get('replaceContentWimtv');
    //echo $_GET['q'];
    $code = explode("viewEmbeddedPlayer/", $_GET['q']);
    //echo $code;

    $output = '';

    if (count($code)>1) {
        $codeArra = explode("/", $code[1]);
        $cI = $codeArra[0];
        $result = db_query("SELECT * FROM {wimtvpro_videos} WHERE contentidentifier='" . $cI . "'");
        $video = $result->fetchAll();
        if (variable_get('nameSkin')!="") {
            $directory = file_create_url('public://skinWim');
        }
        else {
            $directory = base_path() . drupal_get_path('module', 'wimtvpro');
        }

        if (isset($video[0])){

            $output = "<html><head>
		<script type='text/javascript' src='" . $GLOBALS['base_url']  . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/jwplayer.js'></script>
		";
            $output .= "</head><body><div style='text-align:left;'>";
            $output .= "<div id='container'></div>";
            $dimensions = "width:" . variable_get('widthPreview') . ",height:" . variable_get('heightPreview');
            //$dirJwPlayer = drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";
            $dirJwPlayer = $GLOBALS['base_url']  . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";
            if (!isset($video[0]->urlThumbs)) $thumbs[1] = "";
            else $thumbs = explode ('"',$video[0]->urlThumbs);

            $dimension = " width='" . (variable_get('widthPreview')+50) . "px' height='" . (variable_get('heightPreview')+50)  . "px'";
            $embedded = str_replace("+", " " , urlencode("<iframe src='" . $GLOBALS['base_url'] . "/wimtvproplus/viewEmbeddedPlayer/" . $cI . "'" . $dimension . "></iframe>"));
            //echo $_SERVER['HTTP_USER_AGENT'];

            $configFile  = wimtvpro_viever_jwplayer ($_SERVER['HTTP_USER_AGENT'],$cI,$video);

            $output .= "<script type='text/javascript'>jwplayer('container').setup({";
            if (variable_get('nameSkin')!="") $output .= "skin: '" . $directory . "/" . variable_get('nameSkin') . ".zip',";
            else $output .= "skin: '" . $directory . "/skin/default.zip',";
            $output .= "'plugins': {

			   'sharing-3': {

					'code': '" . $embedded   . "'
			   }},";

            /*$output .= " logo: {
            hide: 'false', position:'bottom-right',
            file: '" . $GLOBALS['base_url']  . "/" . drupal_get_path('module', 'wimtvproplus') . "/img/logo_player.png',
            link: '" . $GLOBALS['base_url']  . "'
            }, ";*/

            $urlPlay = explode("$$",$video[0]->urlPlay);

            $output .= $dimensions . "," . $configFile;

            $output .= "image: '" . $thumbs[1] . "',


		});</script>&nbsp;";

            $output .= "</div></body></html>";
        }
        else
            echo "The video is being processed.";


    }
    else {

        echo "Video does not exist.";

    }

    echo $output;

}

function wimtvpro_embeddedPlaylist() {
    $playlist = "";
    $code = explode("viewEmbeddedPlaylist/", $_GET['q']);
    $output = "<html><head><script type='text/javascript' src='" . $GLOBALS['base_url']  . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/jwplayer.js'></script>";
    $output .= "</head><body><div style='text-align:center;'>";
    $output .= "<div id='container'></div>";
    $dirJwPlayer = $GLOBALS['base_url']  . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";
    if (count($code)>1) {

        $arrayVid = explode(",",$code[1]);

        $result = db_query("SELECT * FROM {wimtvpro_playlist} WHERE id = '" . $code[1] . "'");
        $array_playlist=$result->fetchAll();

        $list = $array_playlist[0]->listVideo;

        $options = $array_playlist[0]->option;
        $array_option = explode (",",$options);
        $videoList = explode (",",$list);
        $sql_where  = " 1=2 ";
        for ($i=0;$i<count($videoList);$i++){

            $sql_where .= "  OR contentidentifier='" . $videoList[$i] . "' ";

        }

        $sql_where = "AND (" . $sql_where . ")";

        $result_new = db_query("SELECT * FROM {wimtvpro_videos} WHERE uid='" . variable_get("userWimtv") . "' " . $sql_where);


        $array_videos  = $result_new->fetchAll();
        $array_videos_new_drupal = array();
        for ($i=0;$i<count($videoList);$i++){
            foreach ($array_videos  as $record_new) {
                if ($videoList[$i] == $record_new->contentidentifier){
                    array_push($array_videos_new_drupal, $record_new);
                }
            }
        }


        $playlist = "";
        foreach ($array_videos_new_drupal as $videoT){

            $result_video = db_query("SELECT * FROM {wimtvpro_videos} WHERE contentidentifier='" . $videoT->contentidentifier . "' ");
            $video  = $result_video->fetchAll();

            $configFile  = wimtvpro_viever_jwplayer($_SERVER['HTTP_USER_AGENT'],$videoT->contentidentifier,$video,FALSE);
            if (!isset($videoT->urlThumbs)) $thumbs[1] = "";
            else $thumbs = explode ('"',$videoT->urlThumbs);
            $playlist .= "{" . $configFile . " 'image':'" . $thumbs[1]  . "','title':'" . str_replace ("+"," ",urlencode($videoT->title)) . "'},";

        }

        $dimension = "width:" . variable_get('widthPreview') . ",height:" . variable_get('heightPreview')-10;

        $output .= "<div id='container_playlist'></div>";
        $playlistSize = "30%";
        $dimensions = "width: '100%',";

        if (variable_get('nameSkin')!="") {
            $directory = file_create_url('public://skinWim');
        }
        else {
            $directory = base_path() . drupal_get_path('module', 'wimtvpro');
        }
        $dimension_embedded = " width='" . (variable_get('widthPreview')+50) . "px' height='" . (variable_get('heightPreview')+70)  . "px'";
        $embedded = str_replace("+", " " , urlencode("<iframe src='" . $GLOBALS['base_url'] . "/wimtvproplus/viewEmbeddedPlaylist/" . $code[1] . "'" . $dimension_embedded. "></iframe>"));
        $output .= "<script type='text/javascript'>jwplayer('container_playlist').setup({";

        $output .= "'plugins': {

			   'sharing-3': {

				   'code': '" . $embedded . "'

			   }},";
        /*$output .= " logo: {
        hide: 'false', position:'bottom-right',
        file: '" . $GLOBALS['base_url']  . "/" . drupal_get_path('module', 'wimtvproplus') . "/img/logo_player.png',
        link: '" . $GLOBALS['base_url']  . "'
        }, ";*/
        $options = array();
        foreach ($array_option as $value){
            $array = explode(":",$value);
            if ($array[0]!="")
                $options[$array[0]] = $array[1];

        }
        if ($options["loop"]!="no") $output .= "'repeat':'always',";

        $isiPhone = (bool)strpos($_SERVER['HTTP_USER_AGENT'],'iPhone');
        $isiPad = (bool) strpos($_SERVER['HTTP_USER_AGENT'],'iPad');
        if (!$isiPad  AND !$isiPhone)
            $output .= "'flashplayer':'" . $dirJwPlayer . "',";
        if (variable_get('nameSkin')!="") $output .= "skin: '" . $directory . "/" . variable_get('nameSkin') . ".zip',";
        else $output .= "skin: '" . $directory . "/skin/default.zip',";

        $output .= $dimensions . "'playlist': [" .  $playlist . "],'playlist.position': 'right',	'playlist.size': '" . $playlistSize  . "'});</script>&nbsp;";
    }
    else {
        echo "Videos does not exist.";
    }
    echo $output;
    echo "</div></body></html>";
}
