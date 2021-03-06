<?php

/**
 * This function synchronizes information of the local cache/db with information
 * stored on wimtv server.
 * Currently it only operates on records which are flagged as 
 * pending (status= "OWNED|filename") in the local cache, meaning user has requested 
 * a video upload but video is still being transcoded by the server and not yet accepted.
 */
//PRINT "<H1>NS: HERE</H1>";
class wimtvpro_smartSync {

    static function sync($syncType) {
        $methodName = "sync_" . $syncType;
        if (method_exists(__CLASS__, $methodName)) {
            self::$methodName();
        } else {
            die("<h3>Sync method does not exist!</h3>");
        }
        $error_response = "";
    }

    static function sync_pending() {
        $db_pending_video_array = dbGetUserVideosId(variable_get("userWimtv"), "pending");

        foreach ($db_pending_video_array as $db_record) {
            $error_response = "";
            // NS: We removed the "&" from error response to avoid problem with php 5.4
//            $api_video_detail_response = apiGetDetailsVideo($db_record->contentidentifier, &$error_response);
            $api_video_detail_response = apiGetDetailsVideo($db_record->contentidentifier, $error_response);

            // VIDEO HAS NOT YET TRANSCODED OR NOT EXISTS
            if ($api_video_detail_response == "") {
//                $notReadyString = "The video is not ready yet";
                $notReadyString = "not ready yet";
//                $errorBody = isset($error_response->body) ? $error_response->body : "";                
                $errorBody = $error_response->body;

//                $msg = "";
//                if (is_array($errorBody->body)) {
//                    $msg = implode(",", $errorBody->body);
//                } else if (is_string($errorBody->body)) {
//                    $msg = $errorBody->body;
//                }
//                watchdog("wimtv", $msg);

                $notReady = strstr($errorBody, $notReadyString);
//                watchdog("wimtv-smartsynch", '<pre>' . print_r($errorBody, true) . '</pre>');
//                watchdog("wimtv-smartsynch", '<pre>' . print_r($notReadyString, true) . '</pre>');
                
                if ($notReady == false) {
                    // VIDEO NOT FOUND IN REMOTE SERVER: delete it from local cache
                    dbDeleteVideo($db_record->contentidentifier);
                } else {
                    // VIDEO IS STILL TRANSCODING
                    continue;
                }
            }
            // SYNC RECORD USING RECEIVED REMOTE INFO
            else {
                $api_video_details = $api_video_detail_response->body;
//                var_dump($api_video_details->status);exit;

                $status = $api_video_details->status;
                $title = $api_video_details->title;
                $url_thumbs = '<img src="' . $api_video_details->thumbnailUrl . '"  title="' . $title . '" class="wimtv-thumbnail" />';
                if (isset($api_video_details->streamingUrl)) {
                    $urlVideo = $api_video_details->streamingUrl->streamer . "$$";
                    $urlVideo .= $api_video_details->streamingUrl->file . "$$";
                    $urlVideo .= $api_video_details->streamingUrl->auth_token;
                }
                $duration = $api_video_details->duration;
                $state = null; // WE DO NOT SET THIS STUFF BECAUSE WE ARE UPDATING NEWLY UPLOADED VIDEOS
                $showtime_identifier = null; // WE DO NOT SET THIS STUFF BECAUSE WE ARE UPDATING NEWLY UPLOADED VIDEOS
                $categories = "";
                $valuesc_cat_st = "";
                foreach ($api_video_details->categories as $key => $value) {
                    $valuesc_cat_st .= $value->categoryName;
                    $categories .= $valuesc_cat_st;
                    foreach ($value->subCategories as $key => $value) {
                        $categories .= " / " . $value->categoryName;
                    }
                    $categories .= "<br/>";
                }
                $content_item = $api_video_details->contentId;
                $acquired_identifier = isset($api_video_details->relationId) ? $api_video_details->relationId : "";
                dbUpdateVideo($state, $status, $title, $url_thumbs, $urlVideo, $duration, $showtime_identifier, $categories, $content_item, $acquired_identifier);
            }
        }
    }

}

?>
