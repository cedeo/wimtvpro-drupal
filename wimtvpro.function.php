<?php
/**
  * @file
  * This file is used for the function and utility.
  *
  */
include_once('api/wimtv_api.php');
include_once('wimtvpro.pricing.php');

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

  
  //Select Showtime
  /*$param_st = variable_get("basePathWimtv") . "users/" . variable_get("userWimtv") . "/showtime?details=true";
  $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
  $ch_st = curl_init();
  curl_setopt($ch_st, CURLOPT_URL, $param_st);
  curl_setopt($ch_st, CURLOPT_VERBOSE, 0);
  curl_setopt($ch_st, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch_st, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
  curl_setopt($ch_st, CURLOPT_USERPWD, $credential);
  curl_setopt($ch_st, CURLOPT_SSL_VERIFYPEER, FALSE);
  $details_st  =curl_exec($ch_st);*/

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
function wimtvpro_listThumbs($record_new, $position_new, $replace_content, $showtime, $private, $insert_into_page,$playlist,$st_license) {
  $form = "";
  $my_media= "";
  $content_item_new = $record_new -> contentidentifier;
  $state = $record_new -> state;
  $position = $record_new -> position;
  $status_array = explode("|",$record_new -> status);
  $status = $status_array[0];
  $urlThumbs = $record_new -> urlThumbs;
  $urlPlay = $record_new -> urlPlay;
  $acquider_id = $record_new -> acquiredIdentifier;
  $view_video_state = $record_new -> viewVideoModule;
  $duration = "";
  if (drupal_strlen($record_new -> title) > 20) {
    $title2 = drupal_substr($record_new -> title, 0, 20) .'...';
    $title = $title2;
  }
  else {
    $title = $record_new -> title; 
  }
  $title = stripslashes($title);
  $showtime_identifier = $record_new -> showtimeIdentifier;
  if ((!isset($replace_video)) || ($replace_video == "")) {
    $param_thumb = variable_get("basePathWimtv") . str_replace(variable_get("replaceContentWimtv"), $content_item_new, variable_get("urlThumbsWimtv"));
    $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
    $ch_thumb = curl_init();
    curl_setopt($ch_thumb, CURLOPT_URL, $param_thumb);
    curl_setopt($ch_thumb, CURLOPT_VERBOSE, 0);
    curl_setopt($ch_thumb, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch_thumb, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_setopt($ch_thumb, CURLOPT_USERPWD, $credential);
    curl_setopt($ch_thumb, CURLOPT_SSL_VERIFYPEER, FALSE);
    $replace_video  =curl_exec($ch_thumb);
	$license_type = "";
	if (($showtime_identifier!="") && (count($st_license)>0)){	
		$license_type = $st_license[$showtime_identifier];	
	}
    $isfound = false;
	if (!strstr($replace_video, 'Not Found'))
	  $isfound = true;
    $replace_video = '<img src="' . $replace_video . '" title="' . $title . '" class="" />';
	if ($license_type!="") $replace_video .= '<div class="icon_licence ' .$license_type . '"></div>';
  }
  $wimtvpro_url = "";
  if ($isfound) {
    if ((!$private) && (!$insert_into_page))
      $wimtvpro_url = wimtvpro_checkCleanUrl("", "wimtvpro/embedded/" . $content_item_new . "/" . $showtime_identifier, $GLOBALS['base_path']);
    if ($insert_into_page)
      $wimtvpro_url = wimtvpro_checkCleanUrl("", "wimtvpro/embedded/" . $content_item_new . "/" . $showtime_identifier, $GLOBALS['base_path']);
	$video  = "<a class='wimtv-thumbnail' href='" . $wimtvpro_url . "'>" . $replace_video . "</a>";
  } else {
    $replace_video = false;
  }
  if ($replace_video) {
    include('wimtvpro.form.php');
  if (!$insert_into_page) {
    $my_media .= "<li id='" . $content_item_new . "'>";
  }
  else
    $my_media .= "<li>";
  $form = "";
  if ($private)
    $my_media .= "<div class='thumb ui-state-default'>";
  else
    $my_media .= "<div class='thumbPublic'>";
  if ($state!="showtime")
    $my_media .= "<span title='" . t("Remove") . "' class='icon_remove'></span>";
  if ($private) {
    $my_media .= "<div class='headerBox'><div class='icon'>";
  if ((!$showtime) || (trim($showtime)=="FALSE")) {
    $id  = "";
    $title_add = t("Add to My Streaming");
    $title_remove = t("Remove from My Streaming");
    if ($state!="") {
      //The video is into My Streaming
      $id= "id='" . $showtime_identifier . "'";
      if ($status=="ACQUIRED") {
        $class_r = "AcqRemoveshowtime";
        $class_a = "AcqPutshowtime";
      }
      else{
        $class_r = "Removeshowtime";
        $class_a = "Putshowtime";
      }
      $my_media .= "<span title='" . $title_remove . "' class='icon_" . $class_r . "' " . $id . "></span>";
      $my_media .= "<span title='" . $title_add . "' class='add icon_" . $class_a . "' " . $id . " style='display:none;'></span>";
    }
    else {
      //The video isn't into showtime
      $id = "id='" . $acquider_id . "'";
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
      if ($class_a!="") {
        $my_media .= "<span title='" . $title_remove . "' class='icon_" . $class_r . "' " . $id . " style='display:none;'></span>";
        $my_media .= "<span title='" . $title_add . "' class='add icon_" . $class_a . "' " . $id . " ></span>";
      }
    }
    $form = "<div class='formVideo'>" . $form_st . "</div>";
  }
  else {
    $my_media .= "<span class='icon_RemoveshowtimeInto' title='Remove to My Streaming' id='" . $showtime_identifier . "'></span>";
    $my_media .= "<span class='icon_moveThumbs' title='Change Position'></span>";
    $my_media .= "<span class='icon_viewVideo' rel='" . $view_video_state . "' title='View Thumb in page and/or block'></span>";
  }
  if (isset($status_array[1])) $filename= $status_array[1];
  else $filename = "";
  $my_media .= "<span class='icon_download' id='" . $content_item_new . "|" . $filename . "' title='Download'></span>";
  if ($showtime_identifier!="") {
    $style_view = "";
    $href_view = wimtvpro_checkCleanUrl("admin/config/wimtvpro/", "embedded/" . $content_item_new . "/" . $showtime_identifier);
    $title_view = t("View Video");
	$play=TRUE;
  }
  else {
    $style_view = "";
	if ($urlPlay!="") {
      $href_view = wimtvpro_checkCleanUrl("admin/config/wimtvpro/", "embeddedAll/" . $content_item_new);
      $play=TRUE;
    }
    else $play=FALSE;
	$title_view = t("Preview Video");
  }
  if($play==TRUE)
    $my_media .= "<a class='viewThumb' " . $style_view . " title='" . $title_view . "' href='#' id='" . $href_view . "'><span class='icon_view'></span></a>";
    $my_media .= "	</div>" . $form . "<div class='loader'></div></div>";
  }

    if ($insert_into_page) {
      if ($showtime_identifier!="")
        $my_media .= "<div class='headerBox'><div class='icon'><a class='addThumb' href='#' id='" . $showtime_identifier . "'>" . t("Add") . "</a>  <a class='removeThumb' href='#' id='" . $showtime_identifier . "'>" . t("Remove") . "</a></div></div>";
    }
    $my_media .= $video . "<div class='title'>" . $title . "</div>";
    if ($insert_into_page)
      $my_media .= "W <input style='width:20px;' maxweight='3' class='w' type='text' value='" . variable_get("widthPreview") . "'>px  -  H <input style='width:20px;' maxweight='3' class='h' type='text' value='" . variable_get("heightPreview") . "'>px";
    $my_media .= "</div> </li>";
    $position_new = $position;
  }
  return $my_media;
}

//MY STREAMING: This API allows to list videos in my streaming public area. Even details may be returned
function wimtvpro_detail_showtime($single, $st_id) {
  if (!$single) {
    $url_detail = variable_get("basePathWimtv") . str_replace(variable_get("replaceUserWimtv"), variable_get("userWimtv"), variable_get("urlShowTimeDetailWimtv"));
  }

  else {
    $showtime_item = $st_id;
    $url_embedded =  variable_get("urlShowTimeWimtv") . "/" . variable_get('replaceshowtimeIdentifier') . "/details";
    $replace_content = variable_get('replaceContent');
    $url_detail = str_replace(variable_get('replaceshowtimeIdentifier'), $showtime_item , $url_embedded);
    $url_detail = str_replace(variable_get("replaceUserWimtv"), variable_get("userWimtv"), $url_detail);
    $url_detail = variable_get("basePathWimtv") . $url_detail;
  }
  $st = curl_init();
  curl_setopt($st, CURLOPT_URL, $url_detail);
  curl_setopt($st, CURLOPT_VERBOSE, 0);
  curl_setopt($st, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($st, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
  curl_setopt($st, CURLOPT_SSL_VERIFYPEER, FALSE);
  $array_detail = curl_exec($st);
  curl_close($st);
  return $array_detail;
}

//Return  format url friendly o not
function wimtvpro_checkCleanUrl($base, $url, $back=NULL) {
  if (strpos(request_uri(), '?q=') === FALSE || !empty($_SESSION['clean_url'])) {
    if ($back!=NULL)
      return $back . $url;
    else
      return $url;
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

function wimtvpro_viever_jwplayer($userAgent,$contentId,$video,$viewFlashPlayer=TRUE){

$dirJwPlayer = base_path() . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";
$isiPad = (bool) strpos($userAgent,'iPad');
$urlPlay = explode("$$",$video[0]->urlPlay); 
$isiPhone = (bool) strpos($userAgent,'iPhone');
if (isset($urlPlay[1])) {
  if ($isiPad  || $isiPhone) {
    $urlPlayIPadIphone = "";
    $contentId = $video[0]->contentidentifier;

    $url_video = variable_get("basePathWimtv") . variable_get("urlVideosWimtv") . "/" . $contentId . "?details=true";
    $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL,  $url_video);
    curl_setopt($ch, CURLOPT_USERAGENT,$userAgent); 
    curl_setopt($ch, CURLOPT_VERBOSE, 0);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_setopt($ch, CURLOPT_USERPWD, $credential);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);

    $response = curl_exec($ch);
    $arrayjson   = json_decode($response);

    $urlPlayIPadIphone = $arrayjson->streamingUrl->streamer;
    $configFile = "'file': '" . $urlPlayIPadIphone . "',";	
  }	
  else {
    $configFile  = "";
    if ($viewFlashPlayer==TRUE) $configFile .= "'flashplayer':'" . $dirJwPlayer . "',";
    $configFile .= "'file': '" . $urlPlay[1] . "','streamer':'" . $urlPlay[0] . "',";
  }
  return $configFile;
}
else
return false;

}

function wimtvpro_getThumbs_playlist($list,$showtime=FALSE, $private=TRUE, $insert_into_page=FALSE, $type_public="",$playlist=FALSE) {
  
  
  global $user;
  $replace_content = variable_get("replaceContentWimtv");
  $my_media= "";
  $response_st = "";
  $sql_where  = "  ";
  $videoList = explode (",",$list);
  if ($showtime)
    $sql_where  = "  state='showtime'";
  else
    if ($playlist)
	  $sql_where  = "  1=2";
	else
	  $sql_where  = "  1=1";
  if ($playlist) {
	  for ($i=0;$i<count($videoList);$i++){
	    if ($videoList[$i]!="")
		  $sql_where .= "  OR contentidentifier='" . $videoList[$i] . "' ";
	  }
	$sql_where = "AND (" . $sql_where . ")";  
  }  else {

	  for ($i=0;$i<count($videoList);$i++){
	    if ($videoList[$i]!="")
		  $sql_where .= "  AND contentidentifier!='" . $videoList[$i] . "' ";
	  }
	  
	$sql_where = "AND (" . $sql_where . ")"; 
  
  
  }

  $result_new = db_query("SELECT * FROM {wimtvpro_videos} WHERE uid='" . variable_get("userWimtv") . "' " . $sql_where);

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
  $param_st = variable_get("basePathWimtv") . "users/" . variable_get("userWimtv") . "/showtime?details=true";
  $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
  $ch_st = curl_init();
  curl_setopt($ch_st, CURLOPT_URL, $param_st);
  curl_setopt($ch_st, CURLOPT_VERBOSE, 0);
  curl_setopt($ch_st, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch_st, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
  curl_setopt($ch_st, CURLOPT_USERPWD, $credential);
  curl_setopt($ch_st, CURLOPT_SSL_VERIFYPEER, FALSE);
  $details_st  =curl_exec($ch_st);
  $arrayjson_st = json_decode( $details_st);
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
          $my_media .= wimtvpro_listThumbs($record_new, $position_new, $replace_content, $showtime, $private, $insert_into_page,$playlist,$st_license);
	  }
	  else {
	    $my_media .= wimtvpro_listThumbs($record_new, $position_new, $replace_content, $showtime, $private, $insert_into_page,$playlist,$st_license);
	  }
	}
  }
  
  return $my_media;
}

function wimtvpro_alert_reg() {
    //If user isn't registered or had not insert user and password
    if ((variable_get("userWimtv")=="username") && (variable_get("passWimtv")=="password")){
        return t("If you are not a WIMTV's member yet <a href='@url'>REGISTER</a> or You have not insert the credentials  <a href='@url2'>SIGN IT</a>",array('@url' => url('admin/config/wimtvpro/registration'),'@url2' => url('admin/config/wimtvpro')));
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
