<?php

/**
 * Created by JetBrains PhpStorm.
 * User: walter
 * Date: 14/01/14
 * Time: 17.43
 * To change this template use File | Settings | File Templates.
 */
function wimtvpro_upload_submit($form, &$form_state) {
    $urlfile = $_FILES['files']['tmp_name']["videoFile"];
    $titlefile = check_plain($_POST['titlefile']);

    $cerca = array("'", '"');
    $titlefile = str_replace($cerca, "", $titlefile);
    $titlefile = preg_replace('/[^(\x20-\x7F)]*/', '', $titlefile);
    $titlefile = str_replace($token, "", $titlefile);

    $descriptionfile = check_plain($_POST['descriptionfile']);
    if (isset($_POST['videoCategory']))
        $video_category = filter_xss($_POST['videoCategory']);

    //connect at API for upload video to wimtv

    $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
    $ch = curl_init();
    $url_upload = cms_getWimtvApiUrl() . 'videos';

    curl_setopt($ch, CURLOPT_URL, $url_upload);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array("Content-type: multipart/form-data"));
    curl_setopt($ch, CURLOPT_VERBOSE, 0);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_setopt($ch, CURLOPT_USERPWD, $credential);
    curl_setopt($ch, CURLOPT_POST, TRUE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);

    //add category/ies (if exist)
    $category_tmp = array();
    $subcategory_tmp = array();
    $directory = "public://skinWim";
    $unique_temp_filename = $directory . "/" . time() . '.' . preg_replace('/.*?\//', '', "tmp");
    $unique_temp_filename = str_replace("\\", "/", $unique_temp_filename);
    if (!@move_uploaded_file($urlfile, $unique_temp_filename)) {
        //echo "non copiato";
    }

    $post = array("file" => "@" . drupal_realpath($unique_temp_filename),
        "title" => $titlefile,
        "description" => $descriptionfile,
        "filename" => $_FILES['files']['name']["videoFile"]
    );

    if (isset($video_category)) {
        $id = 0;
        foreach ($video_category as $cat) {
            $subcat = explode("|", $cat);
            $post['category[' . $id . ']'] = $subcat[0];
            $post['subcategory[' . $id . ']'] = $subcat[1];
            $id++;
        }
    }

    watchdog('wimvideo', '<pre>' . print_r($post, TRUE) . '</pre>');
    curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
    $response = curl_exec($ch);
    curl_close($ch);

    $arrayjsonst = json_decode($response);

    if (isset($arrayjsonst->contentIdentifier)) {
        drupal_set_message(t("Upload successful"));
        $query = db_query("INSERT INTO {wimtvpro_videos} (uid,contentidentifier,mytimestamp,position,state, viewVideoModule,status,acquiredIdentifier,urlThumbs,category,title,duration,showtimeIdentifier) VALUES (
    '" . variable_get("userWimtv") . "','" . $arrayjsonst->contentIdentifier . "','" . time() . "',0,'','3','OWNED','','','','" . $titlefile . "','','')");
        $insert = TRUE;
        include(drupal_get_path('module', 'wimtvpro') . "/wimtvpro.sync.php");
        unlink(drupal_realpath($unique_temp_filename));
    }
    else
        form_set_error('', $response);
}

//return vid or contentitem from wimtvpro_videos
function wimtvpro_return($value, $typeReturn) {
    if ($typeReturn == "vid") {
        //$value is a contentitem
        //return vid
        $result = db_query("SELECT vid FROM {wimtvpro_videos} WHERE contentidentifier='" . $value . "'")->fetchAll();
        return $result[0]->vid;
    }
    if ($typeReturn == "contentItem") {
        //$value is a contentitem
        //return vid
        $result = db_query("SELECT contentidentifier FROM {wimtvpro_videos} WHERE vid='" . $value . "'")->fetchAll();
        return $result[0]->contentidentifier;
    }
}

//recove a iframe video wimtv from vid
function wimtvpro_getIframeFromVid($vid) {
    //Recove url video for view it
    $width = "200";
    $height = "150";
//    $contentid = wimtvproplus_return($vid, "contentItem");
    $localVideoItem = dbGetVideosByVid($vid);
    if (!isset($localVideoItem[0]->showtimeIdentifier)) {
        return "<div>" . t("The video isn't into WimVod") . "</div>";
    }
    $contentid = $localVideoItem[0]->contentidentifier;

    $ch = curl_init();

    if (variable_get('nameSkin') != "")
        $skin = "public://skinWim/" . variable_get('nameSkin') . ".zip";
    else
        $skin = $base_url . drupal_get_path('module', 'wimtvpro') . "/skin/skin/default.zip";

    $url = cms_getWimtvApiUrl() . variable_get("urlVideosWimtv") . "/" . $contentid . '/embeddedPlayers';
    $url .= "?get=1&width=" . $width . "&height=" . $height . "&skin=" . $skin;
    $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_VERBOSE, 0);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_setopt($ch, CURLOPT_USERPWD, $credential);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    $response = curl_exec($ch);

    return $response;
}

function wimtvpro_getPlayerFromVid($vid, $typeInline, $videoWidth = null, $videoHeight = null) {
    //Recove url video for view it
    global $base_path;
    drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/jwplayer/jwplayer.js');
//    $width = variable_get('widthPreview');
//    $height = variable_get('heightPreview');
    $width = ($videoWidth != null) ? $videoWidth : variable_get('widthPreview');
    $height = ($videoHeight != null) ? $videoHeight : variable_get('heightPreview');
    $output = "";

    if (variable_get('nameSkin') != "") {
        $directory = 'public://skinWim';
    }
    else
        $directory = drupal_get_path('module', 'wimtvpro');

    $dirJwPlayer = base_path() . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";

    if ($typeInline != "playlist") {
        if ($typeInline == "block") {
            $dimensionsPlayer = "width:" . $width . "px;height:" . $height . "px";
        } else {
            $dimensionsPlayer = "float:left;margin:1px;width:" . $width . "px;height:" . $height . "px";
        }
//        $result = db_query("SELECT * FROM {wimtvpro_videos} WHERE vid='" . $vid . "'");        
//        $video = $result->fetchAll();

        $video = dbGetVideosByVid($vid);
        if (sizeof($video) < 1 && !isset($video[0]->urlPlay)) {
            wimtvpro_smartSync::sync("pending");
//            if ($vid==641){var_dump($video);exit;}
//            $videoUrl = wimtvproplus_updateVideoSync($video[0]->contentidentifier);
        }

//        $result = db_query("SELECT * FROM {wimtvpro_videos} WHERE vid='" . $vid . "'");
//        $video = $result->fetchAll();
        $video = dbGetVideosByVid($vid);
        if (sizeof($video) < 1 && !isset($video[0]->urlPlay)) {
            $videoUrl = "NOTEXIST";
        } else {
            $videoUrl = $video[0]->urlPlay;
        }
        $thumbs = array();
        if ($videoUrl != "") {
            if ($videoUrl == "NOTEXIST") {
                $output = "<div>The video no longer exists.</div>";
            } else {
                if (!isset($video[0]->urlThumbs)) {
                    $thumbs[1] = "";
                } else {
                    $thumbs = explode('"', $video[0]->urlThumbs);
                }

                if (!isset($thumbs[1])) {
                    $thumbs[1] = "";
                }

                $dimensions = "width:" . $width . ",height:" . $height;

                if (variable_get('nameSkin') != "") {
                    $directory = file_create_url('public://skinWim');
                } else {
                    $directory = base_path() . drupal_get_path('module', 'wimtvpro');
                }

                $output .= "<div style='" . $dimensionsPlayer . "'><div id='container_" . $vid . "'></div></div>";
                $output .= "<script type='text/javascript'>jwplayer('container_" . $vid . "').setup({";
                if (variable_get('nameSkin') != "") {
                    $output .= "skin: '" . $directory . "/" . variable_get('nameSkin') . ".zip',";
                } else {
                    $output .= "skin: '" . $directory . "/skin/default.zip',";
                }

                $dimension = " width='" . (variable_get('widthPreview') + 50) . "px' height='" . (variable_get('heightPreview') + 50) . "px'";
                // NS: refactoring signature: contentID is not required!
//                $configFile = wimtvpro_viever_jwplayer($_SERVER['HTTP_USER_AGENT'], $vid, $video);
                $configFile = wimtvpro_viever_jwplayer($_SERVER['HTTP_USER_AGENT'], $video);
                /*
                  $embedded = str_replace("+", " ", urlencode("<iframe src='" . $GLOBALS['base_url'] . "/wimtvproplus/viewEmbeddedPlayer/" . $vid . "'" . $dimension . "></iframe>"));
                  $output .= " logo: {
                  hide: 'false', position:'bottom-right',
                  file: '" . $GLOBALS['base_url']  . "/" . drupal_get_path('module', 'wimtvproplus') . "/img/logo_player.png',
                  link: '" . $GLOBALS['base_url']  . "'
                  }, "; */
                /*
                  $output .= "'plugins': {

                  'sharing-3': {
                  'link': '" . $video[0]->urlPlay . "',
                  'code': '" . $embedded  . "'
                  }},";
                 */


                if (!$configFile) {
                    $output = "The video is being processed.";
                } else {
//                    $thumbnail = $thumbs[1];
                    $html_thumbnail = html_entity_decode($video[0]->urlThumbs);
                    preg_match('/src="([^"]+)"/', $html_thumbnail, $images);
                    $thumbnail = isset($images[1]) ? $images[1] : "";
                    $output .= $dimensions . "," . $configFile . " image: '" . $thumbnail . "',});</script>";
                }
            }
        } else {
            $output = "<div class='fieldWimTvNone' style='" . $dimensionsPlayer . "; text-align:center'>The video is being processed.</div>";
        }
    } else {
        $playlist = "";
        $arrayVid = explode(",", $vid);
        $videosFilter = "1=2 ";
        foreach ($arrayVid as $vids) {
            if (($vids >= 0) && ($vids != "")) {
                $videosFilter .= " OR vid='" . $vids . "'";
            }
        }

        $result = db_query("SELECT * FROM {wimtvpro_videos} WHERE " . $videosFilter);
        $videos = $result->fetchAll();
        foreach ($videos as $video) {
            if (!isset($video->urlPlay))
                $video->urlPlay = "";
            else
                $urlPlay = explode("$$", $video->urlPlay);
            if ($video->urlPlay == "NULL")
                $video->urlPlay = "";
            else
                $urlPlay = explode("$$", $video->urlPlay);
            $thumbs = explode('"', $video->urlThumbs);
            $playlist .= "{'file':'" . $urlPlay[1] . "','streamer':'" . $urlPlay[0] . "','image':'" . $thumbs[1] . "','title':'" . urlencode($video->title) . "'},";
        }
        if (variable_get('nameSkin') != "") {
            $directory = file_create_url('public://skinWim');
        } else {
            $directory = base_path() . drupal_get_path('module', 'wimtvpro');
        }

        $dimensions = "width:100%,height:" . $height;
        $dimensionsPlayer = "float:left;margin:1px;width:" . $width . "px;height:" . $height . "px";
        $output .= "<div id='container_playlist'></div>";
        $playlistSize = "30%";
        $dimensions = "width: '100%',";
        $dimension = " width='" . (variable_get('widthPreview') + 100) . "px' height='" . (variable_get('heightPreview') + 100) . "px'";
        $embedded = str_replace("+", " ", urlencode("<iframe src='" . $GLOBALS['base_url'] . "/wimtvproplus/viewEmbeddedPlaylist/" . $vid . "'" . $dimension . "></iframe>"));

        $output .= "<script type='text/javascript'>jwplayer('container_playlist').setup({";

        /*
          $output .= "
          'plugins': {
          'sharing-3': {

          'code': '" . $embedded  . "'
          }},"; */
        /* $output .= " logo: {
          hide: 'false', position:'bottom-right',
          file: '" . $GLOBALS['base_url']  . "/" . drupal_get_path('module', 'wimtvproplus') . "/img/logo_player.png',
          link: '" . $GLOBALS['base_url']  . "'
          }, "; */

        if (variable_get('nameSkin') != "")
            $output .= "skin: '" . $directory . "/" . variable_get('nameSkin') . ".zip',";
        else
            $output .= "skin: '" . $directory . "/skin/default.zip',";
        $output .= $dimensions . "'flashplayer':'" . $dirJwPlayer . "','playlist': [" . $playlist . "],'playlist.position': 'right',	'playlist.size': '" . $playlistSize . "'});</script>";
    }
    return $output;
}

function wimtvpro_field_is_empty($item, $field) {
    if (isset($item['wimvideo']))
        return empty($item['wimvideo']);
    if (isset($item['wimplaylist']))
        return empty($item['wimplaylist']);
}

function wimtvpro_node_delete($node) {

    /* $valueVid= $node->field_video;

      echo "<script>";
      //Delete videos
      if (isset($valueVid["und"][0]["wimvideo"])){

      $vidVideos = $valueVid["und"][0]["wimvideo"];
      $arrayVid = explode(",",$vidVideos );
      foreach ($arrayVid as $vids) {
      if (($vids>=0) && ($vids!="")){
      $contentItem = wimtvproplus_return($vids,"contentItem");
      $url_delete = variable_get("basePathWimtv") . 'videos';
      $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
      $url_delete .= "/" . $contentItem;
      $ch = curl_init();
      curl_setopt($ch, CURLOPT_URL, $url_delete);
      curl_setopt($ch, CURLOPT_VERBOSE, 0);
      curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "DELETE");
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
      curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      curl_setopt($ch, CURLOPT_USERPWD, $credential);
      curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
      $response = curl_exec($ch);
      curl_close($ch);
      $arrayjsonst = json_decode($response);
      if ($arrayjsonst->result=="SUCCESS"){
      $query = db_delete('{wimtvpro_videos}')
      ->condition("contentidentifier", $contentItem)
      ->execute();
      watchdog("WIMTVPRO","Remove Video " . $contentItem);
      } else {
      watchdog("WIMTVPRO","Error Remove Video " . $contentItem);
      }
      }
      }
      echo "</script>";
      }
     */
}

function wimtvpro_uploadFile() {
//    die(__FILE__);
    global $user;
    $user_name = $user->name;
    $date_hour = date("dmy_His");
    $error = "";
    $msg = "";
    $fileElementName = "files";
    //$fileElementName = $_POST["name"];
    $fieldname = "";
    $x = $_POST["name"];
    $fieldname = str_replace("files[", "", $_POST["name"]);
    $fieldname = str_replace("]", "", $fieldname);

    $titlefile = "";
    $titlefile = $_POST["filetitle"];


    if (!empty($_FILES[$fileElementName]['error'][$fieldname])) {
        switch ($_FILES[$fileElementName]['error'][$fieldname]) {

            case '1':
                $error = 'The uploaded file exceeds the upload_max_filesize directive in php.ini';
                break;
            case '2':
                $error = 'The uploaded file exceeds the MAX_FILE_SIZE directive that was specified in the HTML form';
                break;
            case '3':
                $error = 'The uploaded file was only partially uploaded';
                break;
            case '4':
                $error = 'No file was uploaded.';
                break;

            case '6':
                $error = 'Missing a temporary folder';
                break;
            case '7':
                $error = 'Failed to write file to disk';
                break;
            case '8':
                $error = 'File upload stopped by extension';
                break;
            case '999':
            default:
                $error = 'No error code avaiable';
        }
    } elseif (empty($_FILES[$fileElementName]['tmp_name'][$fieldname]) || $_FILES[$fileElementName]['tmp_name'][$fieldname] == 'none') {
        $error = 'No file was uploaded';
    } else {
        $namefile = $_FILES[$fileElementName]['name'][$fieldname];
        $urlfile = $_FILES[$fileElementName]['tmp_name'][$fieldname];
        if ($titlefile == "") {
            $titlefile = $user_name . "_" . $date_hour;
        }
        $uploadRes = wimtvpro_upload_content($titlefile, $urlfile, $namefile);
        $vid = "";
        $urlThumbs = "";
        $urlVideo = "";
        $titleVideo = "";
        $contentId = "";

        if (is_numeric($uploadRes) && $uploadRes != -1) {
            $insert = true;
            $result = db_query("SELECT * FROM {wimtvpro_videos} WHERE vid='" . $uploadRes . "'");
            $video = $result->fetchAll();
            $urlThumbs = $video[0]->urlThumbs;
            $urlVideo = $video[0]->urlPlay;
            $titleVideo = $video[0]->title;
            $contentId = $video[0]->contentidentifier;
            $showtimeid = $video[0]->showtimeIdentifier;
            $error = "";
            $vid = $uploadRes;
        } else {
            $error = 1;
            $msg = $uploadRes; //"Error: Failed connection with Wimtv.";

            watchdog("Error Upload Video", $error);
        }
    }

    // BUILD JSON RESPONSE
    $jsonResponse = "";
    $jsonResponse .= '{';
    $jsonResponse .= '"error4": "' . $error . '",';
    $jsonResponse .= '"msg": "' . $msg . '",';
    $jsonResponse .= '"vid": "' . $vid . '",';
    $jsonResponse .= '"urlThumbs": "' . str_replace("\"", "\\\"", $urlThumbs) . '",';
    $jsonResponse .= '"urlVideo": "' . $urlVideo . '",';
    $jsonResponse .= '"titleVideo": "' . $titleVideo . '",';
    $jsonResponse .= '"contentId": "' . $contentId . '",';
    $jsonResponse .= '"showtimeid": "' . $showtimeid . '",';
    $jsonResponse .= '"file": "' . $_FILES[$fileElementName]["name"][$fieldname] . '"';
    $jsonResponse .= '}';
    echo $jsonResponse;
}

function my_groovy_theme_preprocess_html(&$vars) {
    drupal_add_css(drupal_get_path('module', 'wimtvpro') . 'css/wimtvpro.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
}

function wimtvpro_output_filter($args) {
    $getThumbs = "<ul class='items' id='FALSE'>" . wimtvproplus_getThumbs_filter(FALSE, $args[0]) . "</ul>";
    return $getThumbs;
}

function wimtvpro_getThumbs_filter($showtime = FALSE, $filter = "", $private = TRUE, $insert_into_page = FALSE, $type_public = "") {
    global $user;
    $replace_content = variable_get("replaceContentWimtv");
    $my_media = "";
    $response_st = "";
    if (($showtime) && ($showtime == "TRUE"))
        $sql_where = " AND state='showtime'";
    else
        $sql_where = "";
    if (!$private) {
        if ($type_public == "block") {
            $sql_where .= " AND ((viewVideoModule='1') OR (viewVideoModule='3')) ";
        }
        if ($type_public == "page") {
            $sql_where .= " AND ((viewVideoModule='2') OR (viewVideoModule='3')) ";
        }
    }
    $result_new = db_query("SELECT * FROM {wimtvpro_videos} WHERE uid='" . variable_get("userWimtv") . "' AND position<>0 " . $sql_where . " ORDER BY Position ASC");
    $array_videos_new_drupal = $result_new->fetchAll();

    $result_new0 = db_query("SELECT * FROM {wimtvpro_videos} WHERE uid='" . variable_get("userWimtv") . "' AND  position=0 " . $sql_where . " ORDER BY contentidentifier ASC");
    $array_videos_new_drupal0 = $result_new0->fetchAll();

    //Add JQuery header
    wimtvpro_install_jquery($showtime, $private);

    $position_new = 1;
    //Select video with position
    if (count($array_videos_new_drupal) > 0) {
        foreach ($array_videos_new_drupal as $record_new) {
            $my_media .= wimtvpro_listThumbs($record_new, $position_new, $replace_content, $showtime, $private, $insert_into_page);
        }
    }
    //Select video with position = 0
    if (count($array_videos_new_drupal0) > 0) {
        foreach ($array_videos_new_drupal0 as $record_new) {
            $my_media .= wimtvpro_listThumbs($record_new, $position_new, $replace_content, $showtime, $private, $insert_into_page);
        }
    }
    return $my_media;
}

function wimtvproplus_embeddedAll_output() {
    $urlEmbedded = variable_get("urlEmbeddedPlayerWimtv");
    $replaceContent = variable_get('replaceContentWimtv');
    //echo $_GET['q'];
    $code = explode("viewEmbeddedPlayer/", $_GET['q']);
    //echo $code;

    $output = '';

    if (count($code) > 1) {
        $codeArra = explode("/", $code[1]);
        $cI = $codeArra[0];
        $result = db_query("SELECT * FROM {wimtvpro_videos} WHERE contentidentifier='" . $cI . "'");
        $video = $result->fetchAll();
        if (variable_get('nameSkin') != "") {
            $directory = file_create_url('public://skinWim');
        } else {
            $directory = base_path() . drupal_get_path('module', 'wimtvpro');
        }

        if (isset($video[0])) {

            $output = "<html><head>
		<script type='text/javascript' src='" . $GLOBALS['base_url'] . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/jwplayer.js'></script>
		";
            $output .= "</head><body><div style='text-align:left;'>";
            $output .= "<div id='container'></div>";
            $dimensions = "width:" . variable_get('widthPreview') . ",height:" . variable_get('heightPreview');
            //$dirJwPlayer = drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";
            $dirJwPlayer = $GLOBALS['base_url'] . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";
            if (!isset($video[0]->urlThumbs))
                $thumbs[1] = "";
            else
                $thumbs = explode('"', $video[0]->urlThumbs);

            $dimension = " width='" . (variable_get('widthPreview') + 50) . "px' height='" . (variable_get('heightPreview') + 50) . "px'";
            $embedded = str_replace("+", " ", urlencode("<iframe src='" . $GLOBALS['base_url'] . "/wimtvproplus/viewEmbeddedPlayer/" . $cI . "'" . $dimension . "></iframe>"));

            // NS: refactoring signature: contentID is not required!
//            $configFile = wimtvpro_viever_jwplayer($_SERVER['HTTP_USER_AGENT'], $cI, $video);
            $configFile = wimtvpro_viever_jwplayer($_SERVER['HTTP_USER_AGENT'], $video);

            $output .= "<script type='text/javascript'>jwplayer('container').setup({";
            if (variable_get('nameSkin') != "")
                $output .= "skin: '" . $directory . "/" . variable_get('nameSkin') . ".zip',";
            else
                $output .= "skin: '" . $directory . "/skin/default.zip',";
            $output .= "'plugins': {

			   'sharing-3': {
				   'link': '" . $embedded . "',
				   'code': '" . $embedded . "'
			   }},";

            /* $output .= " logo: {
              hide: 'false', position:'bottom-right',
              file: '" . $GLOBALS['base_url']  . "/" . drupal_get_path('module', 'wimtvproplus') . "/img/logo_player.png',
              link: '" . $GLOBALS['base_url']  . "'
              }, "; */

            $urlPlay = explode("$$", $video[0]->urlPlay);

            $output .= $dimensions . "," . $configFile;

            $output .= "image: '" . $thumbs[1] . "',


		});</script>";

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

function wimtvproplus_embeddedPlaylist_output() {
    $playlist = "";
    $code = explode("viewEmbeddedPlaylist/", $_GET['q']);
    $output = "<html><head><script type='text/javascript' src='" . $GLOBALS['base_url'] . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/jwplayer.js'></script>";
    $output .= "</head><body><div style='text-align:center;'>";
    $output .= "<div id='container'></div>";
    $dirJwPlayer = $GLOBALS['base_url'] . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";
    if (count($code) > 1) {

        $arrayVid = explode(",", $code[1]);
        $videosFilter = "1=2 ";
        foreach ($arrayVid as $vids) {
            if (($vids >= 0) && ($vids != "")) {
                $videosFilter .= " OR vid='" . $vids . "'";
            }
        }
        $result = db_query("SELECT * FROM {wimtvpro_videos} WHERE " . $videosFilter);
        $videos = $result->fetchAll();
        $urlPlay = explode("$$", $video->urlPlay);
        foreach ($videos as $video) {
            $playlist .= "{'file':'" . $urlPlay[1] . "',streamer:'" . $urlPlay[0] . "','image':'" . $video->urlThumbs . "','title':'" . urlencode($video->title) . "','bufferlength':'0'},";
        }

        $dimensions = "width:" . variable_get('widthPreview') . ",height:" . variable_get('heightPreview');

        $output .= "<div id='container_playlist'></div>";
        $playlistSize = "30%";
        $dimensions = "width: '100%',";

        if (variable_get('nameSkin') != "") {
            $directory = file_create_url('public://skinWim');
        } else {
            $directory = base_path() . drupal_get_path('module', 'wimtvpro');
        }
        $embedded = str_replace("+", " ", urlencode("<iframe src='" . $GLOBALS['base_url'] . "/wimtvproplus/viewEmbeddedPlaylist/" . $vid . "'" . $dimension . "></iframe>"));
        $output .= "<script type='text/javascript'>jwplayer('container_playlist').setup({";

        $output .= "'plugins': {

			   'sharing-3': {
				   'link': '" . $embedded . "',
				   'code': '" . $embedded . "'
			   }},";
        /* $output .= " logo: {
          hide: 'false', position:'bottom-right',
          file: '" . $GLOBALS['base_url']  . "/" . drupal_get_path('module', 'wimtvproplus') . "/img/logo_player.png',
          link: '" . $GLOBALS['base_url']  . "'
          }, "; */

        if (variable_get('nameSkin') != "")
            $output .= "skin: '" . $directory . "/" . variable_get('nameSkin') . ".zip',";
        else
            $output .= "skin: '" . $directory . "/skin/default.zip',";
        $output .= $dimensions . "'playlist': [" . $playlist . "],'playlist.position': 'right',	'playlist.size': '" . $playlistSize . "'});</script>";
    }
    else {
        echo "Videos does not exist.";
    }
    echo $output;
    echo "</div></body></html>";
}

function wimtvpro_insert_videoapp($contentid) {
    //1. Insert Video with content item
    if ($contentid != "") {
        $query = db_query("INSERT INTO {wimtvpro_videos} (uid,contentidentifier,mytimestamp,position,state, viewVideoModule,status,acquiredIdentifier,urlThumbs,category,title,duration,showtimeIdentifier) VALUES (
    '" . variable_get("userWimtv") . "','" . $contentid . "','" . time() . "',0,'','3','OWNED','','','','','','')");

        $vid = wimtvproplus_return($contentid, 'vid');

        watchdog('WimTvPro', 'Video vid is: %vid', array('%vid' => $vid), WATCHDOG_DEBUG);
    } else {
        $vid = "";
    }
    return $vid;
}

function wimtvpro_updateVideoSync($contentid) {
    //Check API
    $videoDetails = cms_getWimtvApiUrl() . "videos/" . $contentid . "?details=true";
    $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $videoDetails);
    curl_setopt($ch, CURLOPT_VERBOSE, 0);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_setopt($ch, CURLOPT_USERPWD, $credential);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    $json_details = curl_exec($ch);
    $array_details = json_decode($json_details);
    if ($array_details) {

        $status = $array_details->status;
        $title = $array_details->title;
        $videosStreaming = "";
        $duration = $array_details->duration;
        $url_thumbs = '<img src="' . $array_details->thumbnailUrl . '"  title="' . $title . '" class="wimtv-thumbnail" />';
        $url_video2 = "";
        if (isset($array_details->streamingUrl)) {
            $streamingUrl = $array_details->streamingUrl;
            $url_video2 = $streamingUrl->streamer . "$$" . $streamingUrl->file . "$$" . $streamingUrl->auth_token;
        }

        //Update DB
        $query = db_update('{wimtvpro_videos}')
                ->fields(array(
                    'state' => 0,
                    'title' => $title,
                    'urlThumbs' => $url_thumbs,
                    'urlPlay' => $url_video2,
                    'duration' => $duration
                ))->condition("contentidentifier", $contentid)
                ->execute();

        return $url_video2;
    } else {

        return "NOTEXIST";
    }
}