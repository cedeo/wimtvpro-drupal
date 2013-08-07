<?php
/**
  * @file
  * This file is use for configured form for manage Playlist.
  *
  */
function wimtvpro_playlist() {

try{

$query = db_query("SELECT `option` FROM  {wimtvpro_playlist} ");

}
catch (Exception $e){
$sql ="ALTER IGNORE TABLE {wimtvpro_playlist} ADD  `option`  TEXT";
$query = db_query($sql);
}

  $view_page = wimtvpro_alert_reg();
  form_set_error("error",$view_page);
  if ($view_page==""){
    global $base_path;
    $urlCallAjax = url("admin/config/wimtvpro/wimtvproCallAjax");
    drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/css/wimtvpro.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
    drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/jquery/css/redmond/jquery-ui-1.8.21.custom.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
    drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/wimtvpro.js');
    drupal_add_js('

jQuery(document).ready(function(){

jQuery(".playlist input.title").click(function() {
jQuery(this).parent().parent().children("td.action").children(".icon_modTitlePlay").show();

});

jQuery(".icon_viewPlay").click(function () {
var id= jQuery(this).parent().attr("rel");
//jQuery(this).colorbox({href:  url_pathPlugin + "pages/embeddedPlayList.php?id=" + id});
});

jQuery(".icon_createPlay").click(function() {
var nameNewPlaylist = jQuery(this).parent().parent().children("td").children("input").val();


//ID = playlist_##
var count = jQuery(".playlist").size();
count  = count + 1; 
//add to DB
jQuery.ajax({
context: this,
url: "' . $urlCallAjax . '", 
type: "GET",
data:{ 
namePlayList : nameNewPlaylist,
namefunction: "createPlaylist"
},
success: function(response){

location.reload();


},
error: function(jqXHR, textStatus, errorThrown){alert(errorThrown);} 
});


});

jQuery(".icon_modTitlePlay").click(function() {
var nameNewPlaylist = jQuery(this).parent().parent().children("td").children("input").val();

//ID = playlist_##
var idPlayList = jQuery(this).parent().attr("rel");

//add to DB
jQuery.ajax({
context: this,
url: "' . $urlCallAjax . '",
type: "GET",
data:{
idPlayList : idPlayList,
namePlayList : nameNewPlaylist,
namefunction: "modTitlePlaylist"
},
success: function(response){
location.reload();
},
error: function(jqXHR, textStatus, errorThrown){alert(errorThrown);} 
});


});


jQuery(".icon_deletePlay").click(function() {
var nameNewPlaylist = jQuery(this).parent().parent().children("td").children("input").val();
//remove from DB
var idPlayList = jQuery(this).parent().attr("rel");
//add to DB
jQuery.ajax({
context: this,
url: "' . $urlCallAjax . '", 

type: "GET",
data:{
idPlayList : idPlayList,
namefunction: "removePlaylist"
},
success: function(response){
location.reload();
},
error: function(jqXHR, textStatus, errorThrown){alert(errorThrown);} 
});

});});', 'inline');


$page = '
<div id="view_stream" class="view">


<table class="wp-list-table widefat fixed posts" style="text-align:center;">

<tr>

<th class="manage-column column-title">Title Playlist</th>
<th class="manage-column column-title">Action</th>
</tr>';

$result = db_query("SELECT * FROM {wimtvpro_playlist} WHERE uid = '" . variable_get("userWimtv") . "' ORDER BY name ASC");
$array_playlist=$result->fetchAll();
$numberPlaylist=count($array_playlist);
$count = 1;
if ($numberPlaylist>0) {

foreach ($array_playlist as $record) {

$listVideo = $record->listVideo;
$arrayVideo = explode(",", $listVideo);
if ($listVideo=="") $countVideo = 0;
else $countVideo = count($arrayVideo);

$page .= '<tr><td class="playlist"><a href="'  . $base_path  . current_path() . '/modify/' . $record->id .  '"><span class="icon_viewPlay"></span></a><input class="title" type="text" value="' . $record->name .  '"/><span class="counter">(' . $countVideo . ')</span></td><td class="action"  rel="' . $record->id . '">';
$page .= '  <span class="icon_modTitlePlay"></span>  <span class="icon_deletePlay"></span>';
$page .='</td></tr>';
$count ++;
}
}

$page .= '<tr><td  class="playlistNew"><input type="text" value="Playlist ' . $count .  '" /></td><td><span class="icon_createPlay"></span></td>';



$page .= '</table>';

//$page = "<div class='clearfix'><ul class='items' id='FALSE'>" . wimtvpro_getThumbs(FALSE) . "</ul></div>";
/*
$playlist = "<div class='system-themes-list-disabled clearfix'><h2>Playlist</h2>";
$playlist .= "</div>";
*/

return  $page;
} 

return  $view_page;

}      

function wimtvpro_playlist_mod($form_state, $id) {
   global $base_url;
  drupal_add_library('system', 'ui.sortable');    
  drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/css/wimtvpro.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/jquery/css/redmond/jquery-ui-1.8.21.custom.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  
  drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/wimtvpro.js');
  drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/colorbox/js/jquery.colorbox.js');
  drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/jquery/colorbox/css/colorbox.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/jwplayer/jwplayer.js');
  drupal_add_js("var url_pathPlugin ='" . $base_url . "';" , "inline");
  drupal_add_js('
		jQuery(document).ready(function(){ 
		
				jQuery("a.wimtv-thumbnail").click( function(){
				    var basePath = Drupal.settings.basePath;
					
				
						var url = jQuery(this).parent().children(".headerBox").children(".icon").children("a.viewThumb").attr("id");
						jQuery(this).colorbox({href:"' . $base_url . '/admin/config/wimtvpro/" + url});
						
					
					
				});
		/*SORTABLE*/      						
		jQuery( ".items_playlist" ).sortable({
			placeholder: "ui-state-highlight"			
		});
		jQuery( ".sortable1 ul#droptrue" ).sortable({
			connectWith: "ul"		
		});
		jQuery( ".sortable2 ul#dropfalse" ).sortable({
			connectWith: "ul",
			deactivate: function( event, ui ) {
				var sort = jQuery(".sortable2 ul#dropfalse").sortable("toArray");
				jQuery(".list").val(sort);
			}
	
		});

		
		
		});	', 'inline');
  
    $identifier = $id['build_info']['args'][0];

    $form['identifier'] = array('#type' => 'hidden', '#value' => $identifier);

	$result = db_query("SELECT * FROM {wimtvpro_playlist} WHERE id = '" . $identifier . "' ORDER BY name ASC");
    $array_playlist=$result->fetchAll();
	
	$option = $array_playlist[0]->option;
	$array_option = explode(",",$option);
	$options = array();
	foreach ($array_option as $value){
	  
	  $array = explode(":",$value);
	 
	  if ($array[0]!="")
	    $options[$array[0]] = $array[1];
	  
	}
	
	$form['list'] = array('#type' => 'hidden', 
	'#attributes' => array(
        'class' => 'list'
	),
	'#value' => $array_playlist[0]->listVideo);
	
	$form['htmltag2'] = array(
	
		'#markup' => variable_get('htmltag2','<h2>' . $array_playlist[0]->name  . '  <a href="' . wimtvpro_checkCleanUrl('admin/config/wimtvpro/','myplaylist/','../../') . '">Back</a></h2><p>Create a playlist of videos (ONLY FREE) to be<br/>inserted within your website</p>')
	
	);
	$form['submit'] = array('#type' => 'submit', '#value' => t('Update'));
	
	if (!isset($options["loop"]))  $options["loop"] = "";
	
	$form['loop'] = 
		array(	'#type' => 'checkbox', 
				'#option' => t('ON'),
				'#default_value' => $options["loop"], 
				'#title' => t('Do you want to see videos cyclically?')
		);
	
	$dimension = " width='" . (variable_get('widthPreview')+50) . "px' height='" . (variable_get('heightPreview')+70)  . "px'";
	$embedded = "<iframe frameBorder='0' src='" . $GLOBALS['base_url'] . "/wimtvpro/viewEmbeddedPlaylist/" . $identifier . "'" . $dimension . "></iframe>";
	
	
	
	
	$page = "<div class='sortable1'>All video (only Free)<ul class='items_playlist' id='droptrue'>" . wimtvpro_getThumbs_playlist($array_playlist[0]->listVideo,TRUE,TRUE,FALSE,"",FALSE) . "</ul></div>";
	$page .= "<div class='sortable2'><b>Video Playlist</b><ul class='items_playlist' id='dropfalse'>" . wimtvpro_getThumbs_playlist($array_playlist[0]->listVideo,TRUE,TRUE,FALSE,"",TRUE) . "</ul>
	<div class='embeddedCode'><label>Click to copy Embedded Code</label><textarea readonly='readonly'  onclick='this.focus(); this.select();  this.execCommand(\"Copy\")'>" . $embedded  . "</textarea></div>
	
	</div>";
	
	$form['htmltag'] = array(
	
		'#markup' => variable_get('htmltag',$page)
	
	);
	
	$form['#validate'][] = 'wimtvpro_playlist_validate';
	$form['#submit'][] = 'wimtvpro_playlist_submit';
	
	
	return $form;
	
}

function wimtvpro_playlist_validate($form, &$form_state) {
    $error="";
	if ($error!="") form_set_error('', check_plain($error));
}
function wimtvpro_playlist_submit($form, &$form_state) {
	
	$item = check_plain($_POST['identifier']);
	$playlist = check_plain($_POST['list']);
	if (isset($_POST['loop']))
		$loopPlaylist = "loop:" . check_plain($_POST['loop']) . ",";
	else 
		$loopPlaylist ="loop:no,";
	
	$sql = "UPDATE {wimtvpro_playlist} SET listVideo='" . $playlist . "' ,  `option`='" . $loopPlaylist . "' WHERE id='" . $item . "'";
    $query = db_query($sql);
	
	drupal_set_message("Update playlist successfully");
	
}





function wimtvpro_embeddedPlaylist_output() {
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

		  $dimensions = "width:" . variable_get('widthPreview') . ",height:" . variable_get('heightPreview')-10;

		  $output .= "<div id='container_playlist'></div>";
		  $playlistSize = "30%";
		  $dimensions = "width: '100%',";

		if (variable_get('nameSkin')!="") {
		  $directory = file_create_url('public://skinWim');
		} 
		else {
			$directory = base_path() . drupal_get_path('module', 'wimtvpro');
		}
		//$embedded = str_replace("+", " " , urlencode("<iframe src='" . $GLOBALS['base_url'] . "/wimtvproplus/viewEmbeddedPlaylist/" . $vid . "'" . $dimension . "></iframe>"));
		$output .= "<script type='text/javascript'>jwplayer('container_playlist').setup({";
		
		/*$output .= "'plugins': {
			
			   'sharing-3': {
				   'link': '" . $embedded . "',
				   'code': '" . $embedded  . "'
			   }},";*/
		/*$output .= " logo: {
		hide: 'false', position:'bottom-right', 
        file: '" . $GLOBALS['base_url']  . "/" . drupal_get_path('module', 'wimtvproplus') . "/img/logo_player.png',
        link: '" . $GLOBALS['base_url']  . "'
		}, ";*/
		
		if (variable_get('nameSkin')!="") $output .= "skin: '" . $directory . "/" . variable_get('nameSkin') . ".zip',";
		
		$option = $array_playlist[0]->option;
		$array_option = explode(",",$option);
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