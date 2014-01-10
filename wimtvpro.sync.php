<?php
/**
  * @file
  * Syncronize the video with wim.tv.
  *
  */

  $url_video = variable_get("basePathWimtv") . variable_get("urlVideosDetailWimtv");
  $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
  $array_all_videos = array();

  //Call API to read all my video into wim.tv
  $response = apiGetVideos();

  watchdog('SYNC - WIMTVPRO', '<pre>' . $response . '</pre>');
  $array_json_videos = json_decode($response);

  if ($array_json_videos==NULL) {
	if (!(isset($insert)))
      form_set_error('', t("Sync: you can not establish a connection with Wimtv. Contact your administrator."));
  }
  else {
    $i=0;
	
    foreach ($array_json_videos -> items as $a) {
      foreach ($a as $key => $value) {
        $array_all_videos[$i][$key] = $value;
      }
      $i++;
    }
    $num = count($array_json_videos);
    if ($num > 0 ) {
      $elenco_video_wimtv = array();
      $elenco_video_drupal = array();
      $result = db_query("SELECT contentidentifier FROM {wimtvpro_videos} WHERE uid = '" . variable_get("userWimtv") . "'");
      $array_videos_drupal=$result->fetchAll();
      foreach ($array_videos_drupal as $record) {
        array_push($elenco_video_drupal, $record -> contentidentifier);
      }
      /* Information detail videos into Showtime */
      $json_st   = wimtvpro_detail_showtime(FALSE, 0);
      $arrayjson_st   = json_decode($json_st);
      $values_st = $arrayjson_st ->items;
      foreach ($values_st as $key => $value) {
        $array_st[$value -> {"contentId"}]["showtimeIdentifier"] = $value -> {"showtimeIdentifier"};
      }
      foreach ($array_all_videos as $video) {
        if (isset($video["acquiredIdentifier"])) $acquired_identifier = $video["acquiredIdentifier"];
        else $acquired_identifier = "";
        $status = $video["status"];
        $title= $video["title"];
		$videosStreaming = "";
        $duration= $video["duration"];
        $content_item =  $video["contentId"];
        $url_thumbs = '<img src="' . $video["thumbnailUrl"] . '"  title="' . $title . '" class="wimtv-thumbnail" />';
        if (isset($video["streamingUrl"])) {
		  $url_video2 = $video["streamingUrl"]->streamer . "$$" . $video["streamingUrl"]->file . "$$" . $video["streamingUrl"]->auth_token;
		}
		else $url_video2 ="";

        $categories = "";
        $valuesc_cat_st = "";
        foreach ($video["categories"] as $key => $value) {
          $valuesc_cat_st .= $value->categoryName;
          $categories .= $valuesc_cat_st;
          foreach ($value -> subCategories as $key => $value) {
            $categories .= " / " . $value -> categoryName;
          }
          $categories .= "<br/>";
        }
        array_push($elenco_video_wimtv, $content_item);
        if (trim($content_item)!="") {
          //Check video exist
          $trovato = FALSE;
          //Check video is into Drupal but not into wimtv
          foreach ($array_videos_drupal as $record) {
            $content_itemAll = $record -> contentidentifier;
            if ($content_itemAll == $content_item) {
              $trovato = TRUE;
            }
          }
          $pos_wimtv="";
          $showtime_identifier ="";
          if (isset($array_st[$content_item])) {
            $pos_wimtv="showtime";
            $showtime_identifier = $array_st[$content_item]["showtimeIdentifier"];
          }
          else {
            $pos_wimtv="";
          }
          if (!$trovato) {
            $query = db_query("INSERT INTO {wimtvpro_videos} (uid,contentidentifier,mytimestamp,position,state, viewVideoModule, status,acquiredIdentifier ,urlThumbs,urlPlay,category,title,duration,showtimeidentifier) VALUES (
	        '" . variable_get("userWimtv") . "','" . $content_item . "','" . time() . "',0,'" . $pos_wimtv . "','3','" . $status . "','" . $acquired_identifier . "','" . $url_thumbs . "','" . $url_video2 . "','" . $categories . "','" . $title . "','" . $duration . "','" . $showtime_identifier . "')");
          }
          else {
            $query = db_update('{wimtvpro_videos}')
            ->fields(array(
            'state' => $pos_wimtv,
            'status' => $status,
            'title' => $title,
            'urlThumbs' => $url_thumbs,
            'urlPlay' => $url_video2,
            'duration' =>  $duration,
            'showtimeidentifier' => $showtime_identifier,
            'category' => $categories
            )) -> condition("contentidentifier", $content_item)
            ->execute();
          }
      }
    }
    $delete_into_drupal = array_diff($elenco_video_drupal, $elenco_video_wimtv);
    foreach ($delete_into_drupal as $key => $value) {
      $query = db_delete('{wimtvpro_videos}')
      ->condition("contentidentifier", $value)
      ->execute();
    }
    if (isset($_GET['sync'])) {
      $return = wimtvpro_getThumbs($_GET['showtime'], TRUE);
      trigger_error($return);
      echo $return;
    }
  }
  else {
    echo t("It isn't element"); //TODO: eh?
  }
}

if (!(isset($insert)))
    die();