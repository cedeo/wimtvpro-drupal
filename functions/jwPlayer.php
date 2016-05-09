<?php

function wimtvpro_get_skin_data() {
    $skinData = array();
    $skinData["skinName"] = "";
    $skinData["styleUrl"] = "";
    $skinData["logoUrl"] = "";

    if (variable_get('nameSkin') != "") {
        // A SKIN HAS BEEN ADDED: OVERRIDE DEFAULT SKIN PATH       
        $skinData["skinName"] = variable_get('nameSkin');
        $skinBaseUrl = file_create_url("public://skinWim");
        $skinBaseDir = drupal_realpath("public://skinWim");
    } else {
        $skinData["skinName"] = "wimtv";
        $skinBaseUrl = file_create_url(drupal_get_path('module', 'wimtvpro') . "/skin");
        $skinBaseDir = drupal_realpath(drupal_get_path('module', 'wimtvpro') . "/skin");
    }


    $skinCssFile = $skinBaseDir . "/" . $skinData["skinName"] . "/" . $skinData["skinName"] . ".css";

    if (file_exists($skinCssFile)) {
        $skinUrl = $skinBaseUrl . "/" . $skinData["skinName"] . "/" . $skinData["skinName"] . ".css";
        $skinData["styleUrl"] = htmlentities($skinUrl);
    }
    $logoUri = $skinBaseDir . "/" . $skinData["skinName"] . "/" . $skinData["skinName"] . ".png";
    $logoUrl = $skinBaseUrl . "/" . $skinData["skinName"] . "/" . $skinData["skinName"] . ".png";

    if (image_get_info($logoUri)) {
        $skinData["logoUrl"] = $logoUrl;
    }


//    var_dump($skinData);die();
    return $skinData;
}

function configurePlayerJS($contentItem, $width = null, $height = null) {
    $player = array();

    $response = apiGetDetailsVideo($contentItem);
    $arrayjson = json_decode($response);

    $player['file'] = $arrayjson->streamingUrl->file;
    $player['streamer'] = $arrayjson->streamingUrl->streamer;
    $player['type'] = "rtmp";
    $player['primary'] = "flash";
    $player['rtmp'] = "{tunnelling: false, fallback: false}";

    $player['width'] = ((isset($width) && $width != "") ? $width : variable_get("widthPreview"));
    $player['height'] = ((isset($height) && $height != "") ? $height : variable_get("heightPreview"));

    $player['image'] = $arrayjson->thumbnailUrl;

    $player['skin'] = "";
    $player['logo'] = "";

    // A SKIN HAS BEEN ADDED: OVERRIDE DEFAULT SKIN PATH
    $skinData = wimtvpro_get_skin_data();
    if ($skinData['styleUrl'] != "") {
        $player['skin'] = "{name : '" . $skinData["skinName"] . "', url : '" . $skinData['styleUrl'] . "'}";
    }

    if ($skinData['logoUrl'] != "") {
        $player['logo'] = "{file : '" . $skinData['logoUrl'] . "', hide : true}";
    }

    $divContainerID = "container-" . rand();
    $playerScript = "
            <script type='text/javascript'>jwplayer.key='2eZ9I53RjqbPVAQkIqbUFMgV2WBIyWGMCY7ScjJWMUg=';</script>
            <script type='text/javascript'>jwplayer('$divContainerID').setup({";
    $playerConfig = getConfFromDataArray($player);
    $playerScript .= $playerConfig;
    $playerScript .= "});</script>";

    $playerScript = "<div id='$divContainerID' ></div>" . $playerScript;
    return $playerScript;
}

function getConfFromDataArray($dataArray) {
    $conf = "";
    foreach ($dataArray as $key => $value) {
        if ($value != "") {
            if ($key != "rtmp" && $key != "skin" && $key != "logo" && $key != "modes" && $key != "playlist" && $key != "listbar") {
                $value = "'" . $value . "'";
            }
            $conf.=$key . ": " . $value . ",";
        }
    }
    return $conf;
}

//function configurePlayer_PlaylistJS($playlist_id, $width = null, $height = null) {
//    $user_agent = $_SERVER['HTTP_USER_AGENT'];
//
//    if (isset($_GET["isAdmin"])) {
//        $is_admin = true;
//    } else {
//        $is_admin = false;
//    }
//
//    $playlistDBData = dbExtractSpecificPlayist($playlist_id);
//    $playlistDBData = $playlistDBData[0];
//    $listVideo = $playlistDBData->listVideo;
//    $title = $playlistDBData->name;
//
//    //Read Data videos
//    $videoList = explode(",", $listVideo);
//
//    $playlist_videos = dbGetUserVideosIn(variable_get("userWimtv"), $videoList);
//    $sorted_videos = array();
//
//    for ($i = 0; $i < count($videoList); $i++) {
//        foreach ($playlist_videos as $record_new) {
//            if ($videoList[$i] == $record_new->contentidentifier) {
//                array_push($sorted_videos, $record_new);
//            }
//        }
//    }
//    $dirJwPlayer = $GLOBALS['base_url'] . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";
//
//    $playlistConf["playlist"] = "";
//    if (count($sorted_videos) > 0) {
//        $playlistConf["playlist"].="[";
//        foreach ($sorted_videos as $video) {
//
//            //Check if browser is mobile
//            $isApple = (bool) strpos($user_agent, 'Safari') && !(bool) strpos($user_agent, 'Chrome');
//            $isiPad = (bool) strpos($user_agent, 'iPad');
//            $isiPhone = (bool) strpos($user_agent, 'iPhone');
//            $isAndroid = (bool) strpos($user_agent, 'Android');
//            $html5 = false;
//            if ($isiPad || $isiPhone || $isAndroid || $isApple) {
//                $html5 = true;
//            }
//
//            if (!$html5) {
//                $playlistConf['modes'] = "[{type:'flash',src:'" . $dirJwPlayer . "'}]";
//            } else {
//                $playlistConf['modes'] = "[{type:'html5'}]";
//            }
//
//
//            $thumbs = array();
//            if (isset($video->urlThumbs)) {
//                $thumbs = explode('"', $video->urlThumbs);
//            }
//            $thumbs[0] = isset($thumbs[0]) ? $thumbs[0] : "";
//            $thumbs[1] = isset($thumbs[1]) ? $thumbs[1] : "";
//
//            $thumb_url = str_replace("\\", "", $thumbs[1]);
//
//            $response = apiGetDetailsVideo($video->contentidentifier);
//            $arrayjson = json_decode($response);
//
//            $playlistConfPlaylistItem = array();
//            $playlistConfPlaylistItem['file'] = $arrayjson->streamingUrl->file;
//            $playlistConfPlaylistItem['streamer'] = $arrayjson->streamingUrl->streamer;
//
////            $playlistConfPlaylistItem['skin'] = "";
////            $playlistConfPlaylistItem['logo'] = "";
//            $playlistConfPlaylistItem['type'] = "rtmp";
//            $playlistConfPlaylistItem['primary'] = $html5 ? "html5" : "flash";
//            $playlistConfPlaylistItem['rtmp'] = "{tunnelling: false, fallback: false}";
//            $playlistConfPlaylistItem['image'] = $thumb_url;
//            $playlistConfPlaylistItem['title'] = str_replace("+", " ", utf8_decode($video->title));
//            $playlistConfPlaylistItem['flashplayer'] = $dirJwPlayer;
//
//
//            $playlistConf["playlist"].="{";
//            foreach ($playlistConfPlaylistItem as $key => $value) {
//                if ($value != "") {
//                    if ($key != "rtmp" && $key != "skin" && $key != "logo") {
//                        $value = "'" . $value . "'";
//                    }
//                    $playlistConf["playlist"].=$key . ": " . $value . ",";
//                }
//            }
//            $playlistConf["playlist"] .= "},";
//        }
//        $playlistConf["playlist"] .= "]";
//    }
//
//    // A SKIN HAS BEEN ADDED: OVERRIDE DEFAULT SKIN PATH
//    $playlistConf['skin'] = "";
//    $playlistConf['logo'] = "";
//    $playlistConf['repeat'] = "always";
//    $playlistConf['fallback'] = "false";
//
//    $playlistConf['width'] = ((isset($width) && $width != "") ? $width : variable_get("widthPreview"));
//    $playlistConf['height'] = ((isset($height) && $height != "") ? $height : variable_get("heightPreview"));
//
////        $playListScript .= "width:1000px, height:100px";
//    $skinData = wimtvpro_get_skin_data();
//    if ($skinData['styleUrl'] != "") {
//        $playlistConf['skin'] = "{name : '" . $skinData["skinName"] . "', url : '" . $skinData['styleUrl'] . "'}";
//    }
//
//    if ($skinData['logoUrl'] != "") {
//        $playlistConf['logo'] = "{file : '" . $skinData['logoUrl'] . "', hide : true}";
//    }
//
////    $playlistConf['primary'] = "flash";
////    $JwPlayerScript = $GLOBALS['base_url'] . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/jwplayer.js";
////                <script type='text/javascript'  src='$JwPlayerScript'></script>
//    $divContainerID = "container-" . $playlist_id . "-" . rand();
//    $playListScript = "
//            <script>jwplayer.key='2eZ9I53RjqbPVAQkIqbUFMgV2WBIyWGMCY7ScjJWMUg=';</script>
//            <script type='text/javascript'>jwplayer('$divContainerID').setup({";
//
//    $playListScript .= getConfFromDataArray($playlistConf) . "});</script>";
//    $playListScript = "<div id='$divContainerID' ></div>" . $playListScript;
//    return $playListScript;
//}

function configurePlayer_PlaylistJS($playlist_id, $width = null, $height = null) {
//Check if browser is mobile
    $user_agent = $_SERVER['HTTP_USER_AGENT'];
    $isApple = (bool) strpos($user_agent, 'Safari') && !(bool) strpos($user_agent, 'Chrome');
    $isiPad = (bool) strpos($user_agent, 'iPad');
    $isiPhone = (bool) strpos($user_agent, 'iPhone');
    $isAndroid = (bool) strpos($user_agent, 'Android');
//    var_dump($user_agent);
//    var_dump($isiPad, $isiPhone, $isAndroid, $isApple);
    if ($isiPad || $isiPhone || $isAndroid || $isApple) {
        return configurePlayer_PlaylistJS_HLS($playlist_id, $width, $height);
    } else {
        return configurePlayer_PlaylistJS_FLASH($playlist_id, $width, $height);
    }
}

function configurePlayer_PlaylistJS_FLASH($playlist_id, $width, $height) {
    $playlistConf = array();
//    if (isset($_GET["isAdmin"])) {
//        $is_admin = true;
//    } else {
//        $is_admin = false;
//    }

    $playlistDBData = dbExtractSpecificPlayist($playlist_id);
    $playlistDBData = $playlistDBData[0];

    $listVideo = $playlistDBData->listVideo;
    $title = $playlistDBData->name;
    //Read Data videos
    $videoList = explode(",", $listVideo);

    $playlist_videos = dbGetUserVideosIn(variable_get("userWimtv"), $videoList);
    $sorted_videos = array();

    for ($i = 0; $i < count($videoList); $i++) {
        foreach ($playlist_videos as $record_new) {
            if ($videoList[$i] == $record_new->contentidentifier) {
                array_push($sorted_videos, $record_new);
            }
        }
    }
    $dirJwPlayer = $GLOBALS['base_url'] . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";

    $playlistConf["playlist"] = "";
    if (count($sorted_videos) > 0) {
        $playlistConf["playlist"].="[";

        foreach ($sorted_videos as $video) {
            // GET VIDEO THUMBNAIL
            // NOTE: urlThumbs is something like: <img src="http://wimtv..." />
            $doc = new DOMDocument();
            $doc->loadHTML(decode_entities($video->urlThumbs));
            $imageTags = $doc->getElementsByTagName('img');
            $thumb_url = $imageTags->length > 0 ? $imageTags->item(0)->getAttribute('src') : "";

            $response = apiGetDetailsVideo($video->contentidentifier);
            $arrayjson = json_decode($response);

            $playlistConfPlaylistItem = array();
            $playlistConfPlaylistItem['file'] = $arrayjson->streamingUrl->file;
            $playlistConfPlaylistItem['streamer'] = $arrayjson->streamingUrl->streamer;
            $playlistConfPlaylistItem['type'] = "rtmp";
            $playlistConfPlaylistItem['primary'] = "flash";
            $playlistConfPlaylistItem['rtmp'] = "{tunnelling: false, fallback: false}";
            $playlistConfPlaylistItem['image'] = $thumb_url;
            $playlistConfPlaylistItem['title'] = str_replace("+", " ", utf8_decode(addslashes($video->title)));
            $playlistConfPlaylistItem['flashplayer'] = $dirJwPlayer;
            $playlistConf["playlist"].="{";
            foreach ($playlistConfPlaylistItem as $key => $value) {
                if ($value != "") {
                    if ($key != "rtmp" && $key != "skin" && $key != "logo") {
                        $value = "'" . $value . "'";
                    }
                    $playlistConf["playlist"].=$key . ": " . $value . ",";
                }
            }
            $playlistConf["playlist"] .= "},";
        }

        $playlistConf["playlist"] .= "]";
    }

    // A SKIN HAS BEEN ADDED: OVERRIDE DEFAULT SKIN PATH
//    $playlistConf['listbar'] = "{position: 'right',size: 180}";
    $playlistConf['skin'] = "";
    $playlistConf['logo'] = "";
    $playlistConf['repeat'] = "always";
    $playlistConf['fallback'] = "false";
//    $playlistConf['playlist.position'] = "right";


    $playlistConf['width'] = ((isset($width) && $width != "") ? $width : variable_get("widthPreview"));
    $playlistConf['height'] = ((isset($height) && $height != "") ? $height : variable_get("heightPreview"));

    $skinData = wimtvpro_get_skin_data();
    if ($skinData['styleUrl'] != "") {
        $playlistConf['skin'] = "{name : '" . $skinData["skinName"] . "', url : '" . $skinData['styleUrl'] . "'}";
    }

    if ($skinData['logoUrl'] != "") {
        $playlistConf['logo'] = "{file : '" . $skinData['logoUrl'] . "', hide : true}";
    }


    $playlistConf['primary'] = "flash";
    $divContainerID = "container-" . $playlist_id . "-" . rand();
    $playListScript = "
            <script>jwplayer.key='2eZ9I53RjqbPVAQkIqbUFMgV2WBIyWGMCY7ScjJWMUg=';</script>
            <script type='text/javascript'>jwplayer('$divContainerID').setup({";


    $playlistConf['modes'] = "[{type:'flash',src:'" . $dirJwPlayer . "'}]";

    $playListScript .= getConfFromDataArray($playlistConf);
    $playListScript .= "});</script>";

    return "<div id='$divContainerID' ></div>" . $playListScript;
}

function configurePlayer_PlaylistJS_HLS($playlist_id, $width, $height) {
    $playlistConf = array();
//    $user_agent = $_SERVER['HTTP_USER_AGENT'];
//    if (isset($_GET["isAdmin"])) {
//        $is_admin = true;
//    } else {
//        $is_admin = false;
//    }

    $playlistDBData = dbExtractSpecificPlayist($playlist_id);
    $playlistDBData = $playlistDBData[0];

    $listVideo = $playlistDBData->listVideo;
    $title = $playlistDBData->name;
    //Read Data videos
    $videoList = explode(",", $listVideo);

    $playlist_videos = dbGetUserVideosIn(variable_get("userWimtv"), $videoList);
    $sorted_videos = array();

    for ($i = 0; $i < count($videoList); $i++) {
        foreach ($playlist_videos as $record_new) {
            if ($videoList[$i] == $record_new->contentidentifier) {
                array_push($sorted_videos, $record_new);
            }
        }
    }
    $playlistConf["playlist"] = "";
    if (count($sorted_videos) > 0) {
        $playlistConf["playlist"].="[";

        foreach ($sorted_videos as $video) {
            if (!isset($video->urlThumbs)) {
                $thumbs[1] = "";
            } else {
                $thumbs = explode('"', $video->urlThumbs);
            }
            // $thumb_url = str_replace("\\", "", $thumbs[1]);
            $thumb_url = isset($thumbs[1]) ? str_replace("\\", "", $thumbs[1]) : "";

            $response = apiGetDetailsVideo($video->contentidentifier);
            $arrayjson = json_decode($response);
            $playlistConfPlaylistItem = array();

            $playlistConfPlaylistItem['file'] = $arrayjson->streamingUrl->streamer;
            $playlistConfPlaylistItem['primary'] = "html5";
            $playlistConfPlaylistItem['fallback'] = "false";
            $playlistConfPlaylistItem['image'] = $thumb_url;
            $playlistConfPlaylistItem['title'] = str_replace("+", " ", utf8_decode(addslashes($video->title)));
            $playlistConf["playlist"].="{";
            foreach ($playlistConfPlaylistItem as $key => $value) {
                if ($value != "") {
                    if ($key != "rtmp" && $key != "skin" && $key != "logo") {
                        $value = "'" . $value . "'";
                    }
                    $playlistConf["playlist"].=$key . ": " . $value . ",";
                }
            }
            $playlistConf["playlist"] .= "},";
        }

        $playlistConf["playlist"] .= "]";
    }

    // A SKIN HAS BEEN ADDED: OVERRIDE DEFAULT SKIN PATH
    $playlistConf['skin'] = "";
    $playlistConf['logo'] = "";
    $playlistConf['repeat'] = "always";
    $playlistConf['fallback'] = "false";

    $playlistConf['width'] = ((isset($width) && $width != "") ? $width : variable_get("widthPreview"));
    $playlistConf['height'] = ((isset($height) && $height != "") ? $height : variable_get("heightPreview"));


    $skinData = wimtvpro_get_skin_data();
    if ($skinData['styleUrl'] != "") {
        $playlistConf['skin'] = "{name : '" . $skinData["skinName"] . "', url : '" . $skinData['styleUrl'] . "'}";
    }

    if ($skinData['logoUrl'] != "") {
        $playlistConf['logo'] = "{file : '" . $skinData['logoUrl'] . "', hide : true}";
    }


    $divContainerID = "container-" . $playlist_id . "-" . rand();
    $playListScript = "
            <script type='text/javascript' src='/wp-content/plugins/wimtvpro/script/jwplayer/jwplayer.js'></script>
            <script>jwplayer.key='2eZ9I53RjqbPVAQkIqbUFMgV2WBIyWGMCY7ScjJWMUg=';</script>
            <script type='text/javascript'>jwplayer('$divContainerID').setup({";

    $playListScript .= getConfFromDataArray($playlistConf);
    $playListScript .= "});</script>";

    return "<div id='$divContainerID' ></div>" . $playListScript;
}
?>

