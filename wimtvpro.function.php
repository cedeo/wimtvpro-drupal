<?php
/**
  * @file
  * This file is used for the function and utility.
  *
  */
include_once('api/wimtv_api.php');
include_once('views/pricing.php');

//Request thumbs videos
function wimtvpro_getThumbs($showtime=FALSE, $private=TRUE, $insert_into_page=FALSE, $type_public="",$playlist=FALSE) {
  global $user;
  $replace_content = variable_get("replaceContentWimtv");
  $my_media= "";
  $response_st = "";
  if (($showtime) && ($showtime=="TRUE")) $sql_where = " AND state='showtime'";
  else $sql_where = "";
  if (!$private) {
    if ($type_public == "block") {
      $sql_where .= " AND ((viewVideoModule='1') OR (viewVideoModule='3')) ";
    }
    if ($type_public == "page") {
      $sql_where .= " AND ((viewVideoModule='2') OR (viewVideoModule='3')) ";
    }
  }
  
  $result = db_query("SELECT * FROM {wimtvpro_videos} WHERE uid='" . variable_get("userWimtv") . "'" . $sql_where);
  $array_count  = $result->fetchAll();
  $n_per_page = 20;
  // Initialize the pager
  $current_page = pager_default_initialize(count($array_count), $n_per_page);
  // Split your list into page sized chunks
  $chunks = array_chunk($array_count, $n_per_page, TRUE);
  // Show the appropriate items from the list
  //$output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page]));
  // Show the pager
  $output = theme('pager', array('quantity',count($array_count)));
  

  if (isset($_GET["page"])) {
	$page = $_GET["page"];
    $sql_limit =  " LIMIT " . ($n_per_page * $page) . " , " . $n_per_page;
  }
  else {
    $page = 0;
    $sql_limit =  " LIMIT " . $n_per_page;
  }
  $query = "SELECT * FROM {wimtvpro_videos} WHERE uid='" . variable_get("userWimtv") . "' " . $sql_where . " ORDER BY Position ASC " . $sql_limit;

  
  $result_new = db_query($query);
  $array_videos_new_drupal  = $result_new->fetchAll();

  /*$result_new0 = db_query("SELECT * FROM {wimtvpro_videos} WHERE uid='" . variable_get("userWimtv") . "' AND  position=0 " . $sql_where . " ORDER BY mytimestamp DESC");
  $array_videos_new_drupal0 = $result_new0->fetchAll();*/

  //Add JQuery header
  wimtvpro_install_jquery($showtime, $private);

  $details_st = apiGetShowtimes();
  $arrayjson_st = json_decode( $details_st);
   watchdog("dettaglio wimtv",$details_st);
  $st_license = array();
  foreach ($arrayjson_st->items as $st){
  	$st_license[$st->showtimeIdentifier] = $st->licenseType;
  }
 
  $position_new=1;
  //Select video with position
  if (count($array_videos_new_drupal )>0) {
    foreach ($array_videos_new_drupal  as $record_new) {	
        $my_media .=  wimtvpro_listThumbs($record_new, $position_new, $replace_content, $showtime, $private, $insert_into_page,$playlist,$st_license);
    }
  }

  
  return $my_media . $output;

}

//Request list of thumbs
function wimtvpro_listThumbs($record_new, $position_new, $replace_content, $showtime, $private, $insert_into_page, $playlist, $st_license, $is_playlist=false) {
    $remove = "";

    if (drupal_strlen($record_new -> title) > 20) {
        $title = drupal_substr($record_new -> title, 0, 20) .'...';
    }
    else {
        $title = $record_new -> title;
    }
    $title = stripslashes($title);

    $contentidentifier = $record_new -> contentidentifier;
    $showtime_identifier = $record_new -> showtimeIdentifier;
    $acquired_id = $record_new -> acquiredIdentifier;

    if ($showtime_identifier) {
        $preview = url("wimtvpro/embedded/" . $contentidentifier . "/" . $showtime_identifier, array('absolute' => TRUE));
    } else {
        $preview = url("admin/config/wimtvpro/embeddedAll/" . $contentidentifier, array('absolute' => TRUE));
        $remove = $contentidentifier;
    }

    $replace_video = apiGetThumbsVideo($contentidentifier);
    $license_type = "";
    if (($showtime_identifier!="") && (count($st_license)>0)){
        $license_type = $st_license[$showtime_identifier];
    }

    $thumbnail = '<img src="' . $replace_video . '" title="' . $title . '" class="" />';

    $state = $record_new -> state;
    $status_array = explode("|",$record_new -> status);
    $status = $status_array[0];
    if ($status=="ACQUIRED") {
        $class_r = "AcqRemoveshowtime";
        $class_a = "Acqputshowtime";
    }
    elseif ($status=="OWNED") {
        $class_r = "Removeshowtime";
        $class_a = "Putshowtime";
    }
    else {
        $class_a ="";
        $class_r ="";
    }
    if ($state!="") {
        //The video is into My Streaming
        $publish_id = $status=="ACQUIRED" ? $acquired_id : $showtime_identifier;
        $rmshowtime_style = "";
        $addshowtime_style = "style='display: none;'";
    } else {
        $publish_id = $status=="ACQUIRED" ? $acquired_id : $contentidentifier;
        $rmshowtime_style = "style='display: none;'";
        $addshowtime_style = "";
    }


    $params = array("title" => $title,
                    "preview" => $preview,
                    "remove" => $remove,
                    "license_type" => $license_type,
                    "thumbnail" => $thumbnail,
                    "contentid" => $contentidentifier,
                    "form_video" => render_template("wimtvpro.form.php"),
                    "publish_id" => $publish_id,
                    "is_field" => $insert_into_page,
                    "rmshowtime_class" => $class_r,
                    "rmshowtime_style" => $rmshowtime_style,
                    "addshowtime_class" => $class_a,
                    "addshowtime_style" => $addshowtime_style);
    if (!$is_playlist){
		if ($private)
            return render_template('templates/table_row.php', $params);
        else
            return render_template('templates/box_row.php', $params);
	} else {
        return render_template('templates/playlist_row.php', $params);
    }
}

//MY STREAMING: This API allows to list videos in my streaming public area. Even details may be returned
function wimtvpro_detail_showtime($single, $st_id) {
  if (!$single)
      return apiGetShowtimes();
  else
      return apiGetDetailsShowtime($st_id);
}

//Return  format url friendly o not
function wimtvpro_checkCleanUrl($base, $url, $back=null) {
  if (strpos(request_uri(), '?q=') === FALSE || !empty($_SESSION['clean_url'])) {
    if ($back!=null)
      return $back . $url;
    else
      return $base . $url;
  }
  else {
    return "?q=" . $base . $url;
  }
}

function wimtvpro_getDateRange($startDate, $endDate, $format="d/m/Y"){
  //Create output variable
  $datesArray = array();
  //Calculate number of days in the range
  $total_days = round(abs(strtotime($endDate) - strtotime($startDate)) / 86400, 0) + 1;
  //Populate array of weekdays and counts
  for($day=0; $day<$total_days; $day++){
    $datesArray[] = date($format, strtotime("{$startDate} + {$day} days"));
  }
  //Return results array
  return $datesArray;
}

function wimtvpro_viever_jwplayer($userAgent, $video, $viewFlashPlayer=true){
    $dirJwPlayer = base_path() . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";
    $isiPad = (bool) strpos($userAgent,'iPad');
    $isiPhone = (bool) strpos($userAgent,'iPhone');
    $isApple = (bool) strpos($userAgent, 'Safari') && !(bool) strpos($userAgent, 'Chrome');
    $isAndroid = (bool) strpos($userAgent,'Android');
    $urlPlay = explode("$$",$video->urlPlay);
    if (isset($urlPlay[1])) {
        if ($isiPad  || $isiPhone || $isApple) {
            $urlPlayIPadIphone = "";
            $contentId = $video[0]->contentidentifier;
            $response = apiGetDetailsVideo($contentId);
            $arrayjson = json_decode($response);

            $urlPlayIPadIphone = $arrayjson->streamingUrl->streamer;
            $configFile = "'file': '" . $urlPlayIPadIphone . "',";
        } else if ($isAndroid) {
            $configFile = "file: '" . $urlPlay[1] . "',";
        } else {
            $configFile  = "";
            if ($viewFlashPlayer==TRUE) $configFile .= "'flashplayer':'" . $dirJwPlayer . "',";
                $configFile .= "'file': '" . $urlPlay[1] . "','streamer':'" . $urlPlay[0] . "',";
        }
        return $configFile;
    }
    return false;

}


function dbBuildVideosIn($listVideos, $in=true) {
    if (count($listVideos)) {
        $where = " AND contentidentifier ";
        if (!$in)
            $where .= "NOT";
        $where .= " IN (";
        foreach ($listVideos as $index=>$video) {
            $where .= "'" . $video . "'";
            if ($index < count($listVideos)-1)
                $where .= ", ";
        }
        $where .= ")";
        return $where;
    }
    return "";
}


function wimtvpro_getThumbs_playlist($list,$showtime=FALSE, $private=TRUE, $insert_into_page=FALSE, $type_public="",$playlist=FALSE) {
  global $user;
  $replace_content = variable_get("replaceContentWimtv");
  $my_media= "";
  $videoList = explode (",",$list);
  if ($showtime) {
    $and_showtime  = "AND state='showtime'";
  } else {
    $and_showtime = "";
  }
  $where = dbBuildVideosIn($videoList, $playlist);
  $result_new = db_query("SELECT * FROM {wimtvpro_videos} WHERE uid='" . variable_get("userWimtv") . "' " . $and_showtime . $where);

  $array_videos  = $result_new->fetchAll();
  $array_videos_new_drupal = array();

  if ($playlist==TRUE) {
	  for ($i=0;$i<count($videoList);$i++){
		 foreach ($array_videos  as $record_new) {
			if ($videoList[$i] == $record_new->contentidentifier){
				array_push($array_videos_new_drupal, $record_new);	
			}
		 }
	  }
  } else {
     $array_videos_new_drupal = $array_videos;
  }

  //Select Showtime
  $details_st = apiGetShowtimes();
  $arrayjson_st = json_decode($details_st);
  $st_license = array();
  foreach ($arrayjson_st->items as $st){
  	$st_license[$st->showtimeIdentifier] = $st->licenseType;
  }
  $position_new=1;
  //Select video with position
  if (count($array_videos_new_drupal )>0) {
    foreach ($array_videos_new_drupal  as $record_new) {
	  if ($showtime) {
	    if ((isset($st_license[$record_new->showtimeIdentifier])) && ($st_license[$record_new->showtimeIdentifier] !="PAYPERVIEW"))
          $my_media .= wimtvpro_listThumbs($record_new, $position_new, $replace_content, $showtime, $private, $insert_into_page,$playlist,$st_license, true);
	  }
	  else {
	    $my_media .= wimtvpro_listThumbs($record_new, $position_new, $replace_content, $showtime, $private, $insert_into_page,$playlist,$st_license, true);
	  }
	}
  }
  
  return $my_media;
}

function wimtvpro_alert_reg() {
    //If user isn't registered or had not insert user and password
    if ((variable_get("userWimtv")=="username") && (variable_get("passWimtv")=="password")){
        return t("If you don't have a WimTV account <a href='@url'>REGISTER</a> | <a href='@url2'>LOGIN</a> with your WimTV credentials",array('@url' => url('admin/config/wimtvpro/registration'),'@url2' => url('admin/config/wimtvpro')));
    } else {
        return "";
    }
}

function getDateRange($startDate, $endDate, $format="d/m/Y"){
    //Create output variable
    $datesArray = array();

    //Calculate number of days in the range
    $total_days = round(abs(strtotime($endDate) - strtotime($startDate)) / 86400, 0) + 1;

    //Populate array of weekdays and counts
    for($day=0; $day<$total_days; $day++)
    {
        $datesArray[] = date($format, strtotime("{$startDate} + {$day} days"));
    }
    return $datesArray;
}
