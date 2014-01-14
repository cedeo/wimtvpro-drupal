<?php
  /** 
    * @file
	* This file is use for module wimtvproplus, create all field .
	*
	*/

//Inizializate Field
function wimtvpro_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  $videos = "";
  $insert = true;
  //include (drupal_get_path('module', 'wimtvpro') . "/wimtvpro.sync.php");

  switch ($display['type']) {
    case 'wimtvpro_wimvideo_block':
      foreach ($items as $delta => $item) {
	    //$contentItem = wimtvproplus_return($item['wimvideo'],"contentItem");
		$arrayVid = explode(",",$item['wimvideo']);
		foreach ($arrayVid as $vid) {
          if (($vid>=0) && ($vid!="")){
            $videos .=  wimtvpro_getPlayerFromVid($vid,'block');
		  }
		}
		$element[$delta] = array(
            '#type' => 'html_tag',
            '#tag' => 'div',
            '#value' => $videos . "<div class='empty'></div>",
        );
		
      }
      break;
	
	case 'wimtvpro_wimvideo_inline':
	 
      foreach ($items as $delta => $item) {
	  	 
	    //$contentItem = wimtvproplus_return($item['wimvideo'],"contentItem");
		$arrayVid = explode(",",$item['wimvideo']);
	
		foreach ($arrayVid as $vid) {
		  if (($vid>=0) && ($vid!="")){
			$videos .=  wimtvpro_getPlayerFromVid($vid,'inline');
		  }
		}
		$element[$delta] = array(
            '#type' => 'html_tag',
            '#tag' => 'p',
            '#value' => $videos . "<div class='empty'></div>",
        );
		
      }	  
      break;

    case 'wimtvpro_wimplaylist_view':
	   drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/jwplayer/jwplayer.js');
	   $idPlaylist = "";
	   $dirJwPlayer = $GLOBALS['base_url']  . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/player.swf";
	   foreach ($items as $delta => $item) {	   
	     $idPlaylist =  $item["wimplaylist"];
	   }
	  if ( $idPlaylist != "" ) {
		  $result = db_query("SELECT * FROM {wimtvpro_playlist} WHERE id = '" . $idPlaylist . "'");
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

		  $output = "<div id='container_playlist'></div>";
		  $playlistSize = "30%";
		  $dimensions = "width: '100%',";

		if (variable_get('nameSkin')!="") {
		  $directory = file_create_url('public://skinWim');
		} 
		else {
			$directory = base_path() . drupal_get_path('module', 'wimtvpro');
		}
        $dimension = " width='" . (variable_get('widthPreview')+50) . "px' height='" . (variable_get('heightPreview')+70)  . "px'";
		$embedded = str_replace("+", " " , urlencode("<iframe frameBorder='0' src='" . $GLOBALS['base_url'] . "/wimtvpro/viewEmbeddedPlaylist/" . $idPlaylist  . "'" . $dimension . "></iframe>"));
		$output .= "<script type='text/javascript'>jwplayer('container_playlist').setup({";
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
		$output .= "'plugins': {
			
			   'sharing-3': {
			
				   'code': '" . $embedded  . "'
			   }},";
		$output .= $dimensions . "'playlist': [" .  $playlist . "],'playlist.position': 'right',	'playlist.size': '" . $playlistSize  . "'});</script>";
      
	    
	  }
	  else {
	    $output= "Playlist does not exist.";
	  }
	   
	   $element[$delta] = array(
            '#type' => 'html_tag',
            '#tag' => 'p',
            '#value' => $output,
        );

      break;
	  
	  
  }
  return $element;
}

function wimtvpro_field_widget_info() {
  return array(
    'wimtvpro_upload' => array(
      'label' => t('Upload video wimtv'),
      'field types' => array('wimtvpro_wimvideo')
    ),
	
		
	'wimtvpro_upload_disabled' => array(
      'label' => t('Disabled Upload video wimtv'),
      'field types' => array('wimtvpro_wimvideo')
    ),
	
	'wimtvpro_playlist' => array(
      'label' => t('Playlist'),
      'field types' => array('wimtvpro_playlist')
    ),

  );
}


function wimtvpro_field_info() {
  return array(
    // We name our field as the associative name of the array.
    'wimtvpro_wimvideo' => array(
      'label' => t('Video Wimtv'),
      'description' => t('MultiUpload'),
      'default_widget' => 'wimtvpro_upload',
      'default_formatter' => 'wimtvpro_wimvideo_inline',
    ),
	
	'wimtvpro_playlist' => array(
      'label' => t('Playlist Wimtv'),
      'description' => t('Playlist Wimtv'),
      'default_widget' => 'wimtvpro_playlist',
      'default_formatter' => 'wimtvpro_wimplaylist_view',
    ),

	
  );
}

function wimtvpro_field_formatter_info() {
  return array(
    // We name our field as the associative name of the array.
    'wimtvpro_wimvideo_inline' => array(
      'label' => t('Inline'),
      'field types' => array('wimtvpro_wimvideo')
    ),
	
	'wimtvpro_wimvideo_block' => array(
      'label' => t('Block'),
      'field types' => array('wimtvpro_wimvideo')
    ),
	
	'wimtvpro_wimplaylist_view' => array(
      'label' => t('Playlist'),
      'field types' => array('wimtvpro_playlist')
    ),
	
  );

}


//FIELD VIDEO WIMTV//
function wimtvpro_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  ini_set('memory_limit', '-1');
  $widget = $element;
  $widget['#delta'] = $delta;
  switch ($instance['widget']['type']) {
  
    case 'wimtvpro_playlist':
	
	  try{
	    $query = db_query("SELECT `option` FROM  {wimtvpro_playlist} ");
      }
	  catch (Exception $e){
        $sql ="ALTER IGNORE TABLE {wimtvpro_playlist} ADD  `option`  TEXT";
        $query = db_query($sql);
      }

	  $identifier = isset($items[$delta]['wimplaylist']) ? $items[$delta]['wimplaylist'] : '';
	  
	  drupal_add_library('system', 'ui.sortable');    
      drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/css/wimtvpro.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
      drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/jquery/css/redmond/jquery-ui-1.8.21.custom.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
      drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/wimtvpro.js');
      drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/colorbox/js/jquery.colorbox.js');
      drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/jquery/colorbox/css/colorbox.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
      drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/jwplayer/jwplayer.js');
      drupal_add_js('
      jQuery(document).ready(function(){ 
		jQuery("a.wimtv-thumbnail").click( function(){
		  var basePath = Drupal.settings.basePath;
		  var url = jQuery(this).parent().children(".headerBox").children(".icon").children("a.viewThumb").attr("id");
		  jQuery(this).colorbox({href:"' . url('admin/config/wimtvpro/')  . '" + url});
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
		jQuery("#titleVideo").keyup(function(){
			// When value of the input is not blank
			if( jQuery(this).val() != "")
			{
				// Show only matching TR, hide rest of them
				jQuery("#droptrue li").hide();
				jQuery("#droptrue li").children("div").children("div.title:contains(\'" + jQuery(this).val() + "\')").parent().parent("li").show();
			}
			else
			{
				// When there is no input or clean again, show everything back
				jQuery("#droptrue li").show();
			}

			
		});

		
      });	', 'inline');
	 //Check number playlist

	$widget += array(
     '#type' => 'fieldset',
	 '#description' => t('PlayList'),
	 '#element_validate' => array('wimtvpro_playlist_validate')
	 );
	 if ($identifier=="") {
	   $result = db_query("SELECT * FROM {wimtvpro_playlist} ORDER BY name ASC");
	   $array_playlist[0]->listVideo = "";
	 }
	 else {
        $result = db_query("SELECT * FROM {wimtvpro_playlist} WHERE id = '" . $identifier . "' ORDER BY name ASC");
	    $array_playlist=$result->fetchAll();
		if (count($array_playlist)>0) {
          $option = $array_playlist[0]->option;
	      $array_option = explode(",",$option);
	      $options = array();
	      foreach ($array_option as $value){
	        $array = explode(":",$value);
	        if ($array[0]!="")
              $options[$array[0]] = $array[1];
	      }	
        } else {
			$options["loop"] = "";
			$array_playlist[0]->listVideo = "";
		}		
    }
	//The title playlist is the title content
	if (!isset($options["loop"]))  $options["loop"] = "";
	$widget['listPlaylist'] = array('#type' => 'hidden', 
	    '#attributes' => array(
        'class' => 'list'
	    ),
	    '#value' => $array_playlist[0]->listVideo);
	$widget['loop'] = 
		array(	'#type' => 'checkbox', 
				'#option' => t('ON'),
				'#default_value' => $options["loop"], 
				'#title' => t('Do you want to see videos cyclically?')
		);
		
		
	$time = trim(time());
	$time = str_replace("0","",$time);
	$time = str_replace(" ","",$time);
	$widget['idPlaylist'] = 
		array(	'#type' => 'hidden', 
				'#value' => ($identifier!="") ? $identifier : $time);
	$embedded = "";
	$page = "<div class='sortable1'>Your video <br/> Search <input id='titleVideo' name='titleVideo' type='text' value=''><ul class='items_playlist' id='droptrue'>" . wimtvpro_getThumbs_playlist($array_playlist[0]->listVideo,FALSE,TRUE,FALSE,"",FALSE) . "</ul></div>";
	$page .= "<div class='sortable2'><b>Video Playlist</b><ul class='items_playlist' id='dropfalse'>" . wimtvpro_getThumbs_playlist($array_playlist[0]->listVideo,FALSE,TRUE,FALSE,"",TRUE) . "</ul>";
	
	if ($identifier!="") {
	  $dimension = " width='" . (variable_get('widthPreview')+50) . "px' height='" . (variable_get('heightPreview')+70)  . "px'";
	  $embedded = "<iframe frameBorder='0' src='" . $GLOBALS['base_url'] . "/wimtvpro/viewEmbeddedPlaylist/" . $identifier . "'" . $dimension . "></iframe>";
	  $page .= "<div class='embeddedCode'><label>Click and copy Embedded Code</label><textarea readonly='readonly'  onclick='this.focus(); this.select();'>" . $embedded  . "</textarea></div>";
	}
	$page .= "</div>";
	
	$widget['htmltag'] = array(
	
		'#markup' => variable_get('htmltag',$page)
	
	);
	$element['wimplaylist'] = $widget;
	break;
  
    case 'wimtvpro_upload':
	
	global $user;
	$vids = isset($items[$delta]['wimvideo']) ? $items[$delta]['wimvideo'] : '';
  
  if ($vids==""){
    $field_name = $field['field_name'];
	if (isset($form_state["input"][$field_name])){
	  $vids = $form_state["input"][$field_name][$langcode][$delta]["wimvideo"]["UploadElencoVideo"];
	}
  }

    $delta = array('#attributes' => array('enctype' => 'multipart/form-data'));

	drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/css/wimtvpro.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
	drupal_add_js('http://ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js',array( 'scope' => 'footer'));
	drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/progressbar/vendor/jquery.ui.widget.js',array( 'scope' => 'footer'));
	drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/progressbar/jquery.iframe-transport.js',array('scope' => 'footer'));
	drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/progressbar/jquery.fileupload.js',array( 'scope' => 'footer'));
	
	drupal_add_js('jQuery(document).ready(function(){
		jQuery.noConflict();
		changeTitle();
		jQuery("form").submit(function(){

			if (jQuery("#wimtv_fileuploaded").val()!="") {
				alert("Attention: You still have to upload the video");
				jQuery("#wimtv_fileuploaded").addClass(" required error ");
				return false;
			} else {
				return true;
			}
		
		});
		
		jQuery(".field_upload").click(function(){
			//ajaxFileUpload(this);		
		});
		

	});
	
	function removeVideo(element){
		var elencoVideo = jQuery(".videosId");
		jQuery.ajax({
			context: this,
			url:   wimtvpro_checkCleanUrl("","admin/config/wimtvpro/wimtvproCallAjax","../../"),
			type: "GET",
			dataType: "html",
			async: false,
			data: "namefunction=RemoveVideo&id=" + jQuery(element).attr("id"),
			beforeSend: function(){jQuery(".icon_save").hide(); jQuery("#progressbar").hide();},
			complete: function(){},
			success: function(response) {

				var json =  jQuery.parseJSON(response);
				var result = json.result;
				if (result=="SUCCESS"){
					jQuery(element).parent().parent().remove();

				}
				alert (json.result + " : " + json.message);
				var vid = jQuery(element).parent().attr("id");
				var val = elencoVideo.val();
				val = val.replace(vid, "");
				val = val.replace(",,", "");
				elencoVideo.val(val);
				jQuery(".icon_save").show();
				jQuery("#progressbar").hide();
			},
			
			error: function(request,error) {
				alert(request.responseText);
				jQuery(".icon_save").show();
			}
		})
	}
	
	
	
	function ajaxFileUpload(obj){
	   
		var file_field = jQuery(obj).parent().parent("div").children("div").children("input.form-file");
		
		wimtvpro_title(jQuery(obj).parent().parent("div").children("div").children("input.form-text"));
		var title_field = jQuery(obj).parent().parent("div").children("div").children("input.form-text").val();
		title_field = title_field.replace("\'","");
		var elencoVideo = jQuery(obj).parent().parent("div").children(".videosId");
		var bar = 0;
		//jQuery("#progressbar").progressbar({value: 0});
		//jQuery("#progressbar #percent").html("");
		if (file_field.val()!=""){
			
			//jQuery("#progressbar").progressbar({value: 0});
			//jQuery(".field_upload").hide();
			//jQuery("#progressbar").show();
				
			//jQuery(".icon_throbber").show();
			var id = file_field.attr("id");
			var name = file_field.attr("name");
			
			
					
			/*
			jQuery.ajaxFileUpload({
				
				url: wimtvpro_checkCleanUrl("","wimtvpro/wimtvproCallUpload","../../"),
				secureuri:false,
				fileElementId:id,
				fileElementName:"fileUploadWimTv",
				
				dataType: "json",
				data:{name:"fileUploadWimTv",filetitle:title_field , id:id},
				
				beforeSend:function(){			
					jQuery(".icon_throbber").show();
				},
				
				success: function (data, status){
				     
					if(data.error4 != ""){
						alert(data.error4);
						jQuery("#progressbar").progressbar({value:0});
						jQuery(".field_upload").show();
						//jQuery("#progressbar").hide();
						jQuery(".icon_throbber").hide();
					} else {
						//Add row into #view_video_add
						jQuery(".icon_throbber").hide();
						var titleVideo = data.titleVideo;
						titleVideo = titleVideo.replace("\'","");
						var row = "<tr>";
						if (data.urlThumbs=="") data.urlThumbs = "<div class=\'none\'></div>";
						row += "<td class=\'video\'>" + data.urlThumbs + "</td>"; //urlVideo
						row += "<td class=\'titlevideo\'><input class=\'title\' type=\'text\' value=\'" +  titleVideo + "\'/><span class=\'icon_modTitleVideo\' rel=\'" + data.vid +  "\'></span><strong class=\'icon_savemodTitleVideo\' rel=\'" + data.vid +  "\'>Apply</strong></td>";
						row += "<td id=\'" + data.vid + "\'><a class=\'icon_remove\' id=\'"+ data.contentId +"\' onClick=\'removeVideo(this)\'></a></td>";
						row += "</tr>";
						
						jQuery("#view_video_add").append(row);
						jQuery("#progressbar").progressbar({value:100});
						jQuery("#progressbar").hide();
						jQuery(".field_upload").show();
						var val = elencoVideo.val();
						if (val!="") val += "," + data.vid;
						else val=data.vid;
						elencoVideo.val(val);
						jQuery("#wimtv_fileuploaded").val("");

					}
					
					changeTitle();
				},
				
				
				error: function (data, status, e){
					jQuery(".field_upload").show();
					jQuery("#progressbar").hide();
					alert("error2:" + e + "-" + status + "-" + data);}
			});
			*/
			
		} else {
			alert("Please select a file and try again.");
		}
		
		return false;
	}
	function wimtvpro_checkCleanUrl(base,url,back){
		var baseUrl = window.location;
		if ( document.location.href.indexOf("?q=") > -1 ) {
			return "?q=" + base + url;
		} else {
			return back + url;
		}
	}

	function wimtvpro_title(obj) {
		
		if (obj.val()=="") {
			var title = jQuery("#edit-title").val();
			
			obj.val(title.replace("\'",""));
		
		}
	
	}
	function wimtvpro_TestFileType(obj) {
      fileName = obj.val();
    fileTypes = [ "", "mov", "mpg", "avi", "flv", "mpeg", "mp4", "mkv", "m4v" ];
    if (!fileName) {
      return;
    }

    dots = fileName.split(".");
    // get the part AFTER the LAST period.
    fileType = "." + dots[dots.length - 1];

    if (fileTypes.join(".").indexOf(fileType.toLowerCase()) != -1) {
    return true;

    } else {
     alert("Please only upload files that end in types: \n\n" + (fileTypes.join(" .")) + "\n\nPlease select a new file and try again.");
     obj.val("");
    }
	
    };
	
	function changeTitle(){
			jQuery(".titlevideo .title").click(function(){
					 jQuery(this).parent().children(".icon_savemodTitleVideo").show();
					 jQuery(this).parent().children(".icon_modTitleVideo").hide();
				});
			jQuery(".titlevideo .icon_modTitleVideo").click(function(){
					 jQuery(this).parent().children(".icon_savemodTitleVideo").show();
					 jQuery(this).parent().children(".icon_modTitleVideo").hide();
					 jQuery(this).parent().children(".title").addClass("focus");
				});
			
			jQuery(".icon_savemodTitleVideo").click(function(){
					 var titleVideo = jQuery(this).parent().children(".title").val();
					   	
	
			 
			 jQuery.ajax({
				context: this,
				url:   wimtvpro_checkCleanUrl("","admin/config/wimtvpro/wimtvproCallAjax","../../"),
				type: "GET",
				dataType: "html",
				async: false,
				data: "namefunction=ModifyTitleVideo&titleVideo=" + titleVideo + "&id=" + jQuery(this).attr("rel"),
			
				complete: function(){},
				success: function(response) {
						
						jQuery(this).hide();
						jQuery(this).parent().children(".icon_modTitleVideo").show();
						alert ("Update successful");
						jQuery(this).parent().children(".title").removeClass("focus");
				},
				
				error: function(request,error) {
					alert(request.responseText);
					jQuery(this).hide();
					jQuery(this).parent().children(".icon_modTitleVideo").show();
					jQuery(this).parent().children(".title").removeClass("focus");
				}
			})
			 
			 
		  });	
	    }

	' ,'inline');

	drupal_add_js('jQuery(document).ready(function(){
		jQuery.noConflict();
		jQuery(function () {

		
			
			var file_field = jQuery("input#wimtv_fileuploaded");
			var id = file_field.attr("id");
			var name = file_field.attr("name");
			var url =   wimtvpro_checkCleanUrl("","wimtvpro/wimtvproCallUpload","../../");

			
			jQuery("#wimtv_fileuploaded").fileupload({
				dataType: "json",
				done: function (e, data,jqXHR) {
					var response = data.jqXHR.responseText;
					var elencoVideo = jQuery(".videosId");
					response= jQuery.parseJSON(response);
					if(response.error4 != ""){
						alert(response.error4);
						
					} else {
						//Add row into #view_video_add
						var titleVideo = response.titleVideo;
						titleVideo = titleVideo.replace("\'","");
						var row = "<tr>";
						if (response.urlThumbs=="") response.urlThumbs = "<div class=\'none\'></div>";
						row += "<td class=\'video\'>" + response.urlThumbs + "</td>"; //urlVideo
						row += "<td class=\'titlevideo\'><input class=\'title\' type=\'text\' value=\'" +  titleVideo + "\'/><span class=\'icon_modTitleVideo\' rel=\'" + response.vid +  "\'></span><strong class=\'icon_savemodTitleVideo\' rel=\'" + response.vid +  "\'>Apply</strong></td>";
						row += "<td id=\'" + response.vid + "\'><a class=\'icon_remove\' id=\'"+ response.contentId +"\' onClick=\'removeVideo(this)\'></a></td>";
						row += "</tr>";
						
						jQuery("#view_video_add").append(row);
						jQuery(".field_upload").show();
						var val = elencoVideo.val();
						if (val!="") val += "," + response.vid;
						else val=response.vid;
						elencoVideo.val(val);
						jQuery("#wimtv_fileuploaded").val("");
						changeTitle();

					}
				
					jQuery("#progress .bar").attr("style","width:0");
					jQuery("#progress .percent").html("");
					data.jqXHR.abort();
				},
				type: "GET",
				url: url,
				multipart:true,
				 add: function (e, data) {
					var title_field = jQuery(".fileName_text").val();
					data.fileInput = jQuery(this);

					data.formData = {"name":jQuery(this).attr("name"),"filetitle":title_field , "id":id};
					data.submit();
		
					jQuery(".field_upload").hide();
				},
				
					
				progressall: function (e, data) {
					var progress = parseInt(data.loaded / data.total * 100, 10);
					jQuery("#progress .bar").css(
						"width",
						progress + "%"
					)
					
					jQuery("#progress .percent").html(progress + "%");
				},
				
				processfail:	function (e, data) { 
					alert (data);
				}
			});

			
		});
	});
	
		
	
	' ,array('type' => 'inline', 'scope' => 'footer'));
	
	
    // Make this a fieldset with the three text fields.
    $rows  = "";
    if (strlen($vids)>0){
		 drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/wimtvpro.js');
	  $arrayVid = explode(",",$vids);
	  foreach ($arrayVid as $vid) {
	    if ((trim($vid)!="") & (is_numeric($vid))) {
		//$videos = wimtvproplus_getPlayerFromVid($vid);
     	  $result = db_query("SELECT * FROM {wimtvpro_videos} WHERE vid='" . $vid . "'");
		  $video = $result->fetchAll();
		  if (isset($video[0]->urlThumbs)) $urlThumbs = $video[0]->urlThumbs;
		  else $urlThumbs = "<div class='none'></div>";
		  if (isset( $video[0]->urlPlay)) $urlVideo = $video[0]->urlPlay;
		  else $urlVideo = "not yet available";
		  if (isset($video[0]->title)) $titleVideo = $video[0]->title;
		  else $titleVideo = "Not title";
		  if (isset($video[0]->contentidentifier)) $contentId = $video[0]->contentidentifier;
		  else $contentId = "";
		  $rows .= "<tr>";
		  $rows .= "<td class='video'>" . $urlThumbs  . "</td>"; //urlVideo
		  $rows .= "<td class='titlevideo'><input class='title' type='text' value='" . $titleVideo . "'/><span class='icon_modTitleVideo' rel='" . $vid . "'></span><strong class='icon_savemodTitleVideo' rel='" . $vid . "'>Apply</strong></td>";
		  if (isset($video[0]->contentidentifier)) {
		    $contentId = $video[0]->contentidentifier;
			global $base_url;
			$rows .= "<td id='" . $vid . "'><a class='icon_remove' id='" . $contentId . "' onClick='removeVideo(this)'></a>
			
			<!--span onClick='downloadVideo(\"" . $contentId . "\")'>Download2</span-->
			
			</td>";
			
		  }
		  else
		    $rows .= "<td id=''></td>";
		  $rows .= "</tr>";
        }
      }
    }
    $widget += array(
     '#type' => 'fieldset',
	 '#description' => t('Pick a video file to upload.'),
	 '#element_validate' => array('wimtvpro_upload_validate'),
	 );
	 $widget["titleVideo"] =array(
	 '#type' => 'textfield',
	 '#title' => t('Video Title'), 
	 '#attributes' => array('class'=> array('fileName_text'),'onclick' => 'wimtvpro_title(jQuery(this))'),
	 );
	 $widget["Upload"] =array(
	 '#type' => 'file',
	 '#title' => t('Add a new video'), 
	 '#description' =>  t('Allowed file extensions ') . t(': <b>mov mpg avi flv mpeg mp4 mkv m4v</b>'),
	 '#attributes' => array('onchange' => 'wimtvpro_TestFileType(jQuery(this))',
	 						'id'=>'wimtv_fileuploaded'),
	 
	 );
	 $widget["UploadElencoVideo"] = array(
	 '#type' => 'hidden',
	 '#default_value' => $vids,
	 '#delta' => $delta,
	 '#attributes' => array('class' => 'videosId'),
	 );
	 $widget["Button"] = array(
	 //'#markup' => variable_get('htmltag3',t("<div class='action'><span class='field_upload icon_save'>Upload</span><div id='progress'><div class='percent'></div><div class='bar' style='width: 0%;'></div></div></div>"))
	 '#markup' => variable_get('htmltag3',t("<div class='action'><div id='progress'><div class='percent'></div><div class='bar' style='width: 0%;'></div></div></div>"))
	 );
	 $widget["Table"] = array(
	 '#suffix' => '<table id="view_video_add">' . $rows . '</table>',
	 );
	 
	 
	 
	 $element['wimvideo'] = $widget;
  break;
  case 'wimtvpro_upload_disabled':
   $vids = isset($items[$delta]['wimvideo']) ? $items[$delta]['wimvideo'] : '';
  
  if ($vids==""){
    $field_name = $field['field_name'];
	if (isset($form_state["input"][$field_name])){
	  $vids = $form_state["input"][$field_name][$langcode][$delta]["wimvideo"]["UploadElencoVideo"];
	}
  }

  $delta = array('#attributes' => array('enctype' => 'multipart/form-data'));
	$rows  = "";
	if (strlen($vids)>0){
	  $arrayVid = explode(",",$vids);
	  foreach ($arrayVid as $vid) {
	    if ((trim($vid)!="") & (is_numeric($vid))) {
		  //$videos = wimtvproplus_getPlayerFromVid($vid);
		  $result = db_query("SELECT * FROM {wimtvpro_videos} WHERE vid='" . $vid . "'");
		  $video = $result->fetchAll();
		  if (isset($video[0]->urlThumbs)) $urlThumbs = $video[0]->urlThumbs;
		  else $urlThumbs = "<div class='none'></div>";
		  if (isset( $video[0]->urlPlay)) $urlVideo = $video[0]->urlPlay;
		  else $urlVideo = "not yet available";
		  if (isset($video[0]->title)) $titleVideo = $video[0]->title;
		  else $titleVideo = "Not title";
		  if (isset($video[0]->contentidentifier)) $contentId = $video[0]->contentidentifier;
		  else $contentId = "";
	      $rows .= "<tr>";
		  $rows .= "<td class='video'>" . $urlThumbs  . "</td>"; //urlVideo
		  $rows .= "<td>" . $titleVideo . "</td>";
		  $rows .= "</tr>";
		}
      }
    } 
  $widget += array(
  '#type' => 'fieldset',
  // #delta is set so that the validation function will be able
  // to access external value information which otherwise would be
  // unavailable.
  '#description' => t('Pick a video file to upload.'),
  '#element_validate' => array('wimtvpro_upload_validate'),
  );
  $widget["Upload"] =array(
  '#type' => 'file',
  '#title' => t('Add a new file'), 
  '#description' =>  t('Allowed file extensions ') . t(': <b>mov mpg avi flv mpeg mp4 mkv m4v</b>'),
  '#disabled' => TRUE,
  );
  $widget["UploadElencoVideo"] = array(
  '#type' => 'hidden',
  '#default_value' => $vids,
  '#delta' => $delta,
  '#attributes' => array('id'=>'wimtv_fileuploaded','class' => 'videosId'),
  );
  $widget["Table"] = array(
  '#suffix' => '<table id="view_video_add">' . $rows . '</table>',
  );
  $element['wimvideo'] = $widget;
  break;
}


return $element;

}

//Function for upload video into content type custom
//Call this function and passed title and temp file $_FILES['files']['tmp_name']["videoFile"];
function wimtvpro_upload($titlefile,$urlfile,$namefile) {
  set_time_limit(0);
  //connect at API for upload video to wimtv
  $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
  $ch = curl_init();
  $url_upload = variable_get("basePathWimtv") . 'videos';
  
  $directory = "public://skinWim";
  $unique_temp_filename = $directory .  "/" . time() . '.' . preg_replace('/.*?\//', '',"tmp");
  $unique_temp_filename = str_replace("\\" , "/" , $unique_temp_filename);
  if (!@move_uploaded_file( $urlfile , $unique_temp_filename)) {
	watchdog('UPLOAD VIDEO WIMTVPRO','Error: not working move_uploaded_file' .  $urlfile ."," . $unique_temp_filename);
  }
 
  $post= array("file" => "@" .  drupal_realpath($unique_temp_filename),
  "title" =>  $titlefile,
  "filename" => $namefile
  );
  
  watchdog('UPLOAD VIDEO WIMTVPRO','Upload File (server): ' . drupal_realpath($unique_temp_filename));
  watchdog('UPLOAD VIDEO WIMTVPRO','Title file: ' . $titlefile);
  watchdog('UPLOAD VIDEO WIMTVPRO','Filename: ' . $namefile);
  
  $errorCurl = "";
  curl_setopt($ch, CURLOPT_URL, $url_upload);
  curl_setopt($ch, CURLOPT_HTTPHEADER, array("Content-type: multipart/form-data"));
  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
  curl_setopt($ch, CURLOPT_USERPWD, $credential);
  curl_setopt($ch, CURLOPT_POST, TRUE);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
  $errorCurl = curl_error ($ch);
  // if ($errorCurl!="") watchdog('error curl - WIMTVPROPLUS',curl_error ($ch));
  
  curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
  $response = curl_exec($ch);
  curl_close($ch);
  
  // watchdog('CONTENT ID - WIMTVPROPLUS', '<pre>' . $response . '</pre>');
  
  $arrayjsonst = json_decode($response);
  if (isset($arrayjsonst->contentIdentifier)) {
		
		unlink(drupal_realpath($unique_temp_filename));
		//drupal_set_message( t("Upload Successfully") );
		
		$id = db_insert('{wimtvpro_videos}') ->fields(array(
		'uid' => variable_get("userWimtv") ,
		'contentidentifier' => $arrayjsonst->contentIdentifier,
		'mytimestamp' => time(),
		'position' => 0,
		'state'  => '',
		'viewVideoModule' => '3',
		'status' => 'OWNED',
		'acquiredIdentifier' => '',
		'urlThumbs' => '',
		'category' => '',
		'title'  => $titlefile,
		'duration' => '',
		'showtimeIdentifier' => ''
		)) ->execute();
	 
  } else  {
	if (isset($arrayjsonst->message))
		$error = $arrayjsonst->message;
	else
		$error = "Error: Failed connection with Wimtv.";
	watchdog ("WimTv", "Error Upload Video: " . $error );
	return  $error;
  }
  return $id;
}

function wimtvpro_upload_validate($element, &$form_state, $form) {

  $delta = $element['#delta']; // TODO: Isn't there a better way to find out which element?
  $field = $form_state['field'][$element['#field_name']][$element['#language']]['field'];
  $field_name = $field['field_name'];
  $file_video = "";
  $vids = 0;
  if (isset($form_state['values'][$field_name][$element['#language']][$delta]['wimvideo'])) {
    $values = $form_state['values'][$field_name][$element['#language']][$delta]['wimvideo'];
    $vids = $values["UploadElencoVideo"];
  }
  //if ($file_video!="") form_set_error($values["Upload"],"You still have a video to upload.");
  form_set_value($element, $vids, $form_state);
}
//END field video wimtv

//FIELD PLAYLIST //
//Function for create Playlist
function wimtvpro_playlist_validate($element, &$form_state, $form) {
  $delta = $element['#delta']; // TODO: Isn't there a better way to find out which element?
  $field = $form_state['field'][$element['#field_name']][$element['#language']]['field'];
  $field_name = $field['field_name'];
  $id_playlist = 0;
  $loopPlaylist = "loop:no,";
  $list_video = "";
  if (isset($form_state['values'][$field_name][$element['#language']][$delta]['wimplaylist'])) {
    $values = $form_state['values'][$field_name][$element['#language']][$delta]['wimplaylist'];
    $id_playlist = $values["idPlaylist"];
	$list_video = $form_state["input"][$field_name][$element['#language']][$delta]["wimplaylist"]["listPlaylist"];

	if ($values["loop"]!=0)
		$loopPlaylist = "loop:" . $values["loop"] . ",";
  }
  //Request Title, video's list, IdPlaylist, Option

  $name = $form_state["input"]["title"];
  
  if ($name!=""){
    $control_sql =  "SELECT * FROM {wimtvpro_playlist} WHERE id = '" . $id_playlist . "'";
	$control_query = db_query($control_sql);
	$result = $control_query->fetchAll();
	if (count($result)>0) {
	  //Exist, Update
	  $sql = "UPDATE {wimtvpro_playlist} SET listVideo='" . $list_video . "', name='" . mysql_escape_string($name) . "', `option`='" . $loopPlaylist . "' WHERE id = '" . $id_playlist . "'";
	} 
	else {
	  //Not exist, Insert
      $sql = "INSERT INTO {wimtvpro_playlist} (uid,listVideo,name,id,`option`) VALUES ('" . variable_get("userWimtv") . "' ,'" . $list_video . "','" . mysql_escape_string($name) . "','" . $id_playlist . "','" . $loopPlaylist . "')";
    }
	$query = db_query($sql);

	form_set_value($element, $id_playlist, $form_state);
  } 
}