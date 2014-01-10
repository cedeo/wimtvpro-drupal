<?php
/**
  * @file
  * This file is use for configured form for upload new video.
  *
  */
// Install Form for video upload
function wimtvpro_upload_form() {
drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/css/wimtvpro.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
drupal_add_js('
function viewCategories(obj){
 jQuery("#addCategories").html("You are selected");
 var selectedArray = new Array();
 count = 0;
 for (i=0; i<obj.options.length; i++) {
    if (obj.options[i].selected) {
      selectedArray[count] = obj.options[i].value;
      valueSelected = obj.options[i].value;
      count++;
      jQuery("#addCategories").append("<br/>" + valueSelected);
    }
  }
};', 'inline');
drupal_add_js('
function wimtvpro_TestFileType() {
fileName = jQuery("input[name=\"files[videoFile]\"]").val();
fileTypes = [ "", "mov", "mpg", "avi", "flv", "mpeg", "mp4", "mkv", "m4v" ];
if (!fileName) {
return;
}

dots = fileName.split(".");
// get the part AFTER the LAST period.
fileType = "." + dots[dots.length - 1];

if (fileTypes.join(".").indexOf(fileType.toLowerCase()) != -1) {
return TRUE;

} else {
alert("Please only upload files that end in types: \n\n"
+ (fileTypes.join(" ."))
+ "\n\nPlease select a new file and try again.");
jQuery("input[name=\"files[videoFile]\"]").val("");
}
}
Drupal.behaviors.myBehavior = {
attach: function (context, settings) {
jQuery("#edit-submit").bind("click", function() {
jQuery(".icon_sync2").show();
jQuery(".form-submit").hide();

});
}
};', 'inline');

return drupal_get_form('wimtvpro_upload');

}

// Form for video upload
function wimtvpro_upload($form_state) {

$view_page = wimtvpro_alert_reg();
 form_set_error("error",$view_page);

$form = array('#attributes' => array('enctype' => 'multipart/form-data'));

 if ($view_page==""){
$form['titlefile'] = array(
'#type' => 'textfield',
'#title' => t('Title Video'),
'#default_value' => variable_get('titlefile', ''),
'#size' => 100,
'#maxlength' => 200,
'#required' => TRUE,
 '#weight' => 1,
);
$form['descriptionfile'] = array(
'#type' => 'textarea',
'#title' => t('Description Video'),
'#default_value' => variable_get('descriptionfile', ''),
'#size' => 100,
'#maxlength' => 800,
'#required' => FALSE,
 '#weight' => 2,
);


$form['videoFile'] = array(
'#type' => 'file',
'#title' => t('Upload video'),
'#description' => t('Pick a video file to upload.'),
'#required' => TRUE,
'#attributes' => array(
'onchange' => 'wimtvpro_TestFileType()'),
 '#weight' => 3,
);


$url_categories = variable_get("basePathWimtv") . "videoCategories";
$response = apiGetVideoCategories(); // curl_exec($ch);
$category_json = json_decode($response);
$category = array();
if (isset($category_json)) {
	foreach ($category_json as $cat) {
	  foreach ($cat as $sub) {
		foreach ($sub->subCategories as $subname) {
		  $category[$sub->name][$sub->name . "|" . $subname->name] = $subname->name;
		}
	  }
	}
}
$form['videoCategory'] = array(
'#type' => 'select',
'#title' => t('Category-Subcategory'),
'#options' => $category,
'#default_value' => variable_get('videoCategory'),
'#multiple' => TRUE,
'#required' => FALSE,
'#maxlength' => 400,
'#size' => 15,
'#description' => t('(Multiselect with CTRL)'),
'#attributes' => array('onchange' => 'viewCategories(this);'),
 '#weight' => 4,
);

$form['htmltag2'] = array(
'#markup' => variable_get('htmltag2',
"<p class='description' id='addCategories'></p>"),
 '#weight' => 5,
);


$form['submit'] = array(
'#type' => 'submit',
'#value' => t('Upload'),
 '#weight' => 6,


);

$form['#validate'][] = 'wimtvpro_upload_validate';
$form['#submit'][] = 'wimtvpro_upload_submit';

$form['htmltag'] = array(
'#markup' => variable_get('htmltag',
"<div class='action'><span class='icon_sync2' style='display:none;'>Loading...</span>
Do not leave this page until the file upload is not terminated
</div>"), 
 '#weight' => 7,
);

} 
return $form;
}

// Validate for video upload
//TODO: test this code!
function wimtvpro_upload_validate($form, &$form_state) {


  /*
  $file = $_FILES['files']['name']["videoFile"];
   
  $error = "";
  $titlefile = check_plain($_POST['titlefile']);
  if (empty($file)) {
    $error .= t('You must upload a file.');
  }
  if (empty($titlefile)) {
    $error .=  t('You must write a title.');
  }


  if ($error!="") {
    form_set_error('', check_plain($error));
	return false;
  }
 
  $file = $_FILES['files']['name']["videoFile"];
  $urlfile  = $_FILES['files']['tmp_name']["videoFile"];
  $titlefile = check_plain($_POST['titlefile']);
  $descriptionfile = check_plain($_POST['descriptionfile']);
  if (isset($_POST['videoCategory']))
    $video_category = filter_xss($_POST['videoCategory']);
  set_time_limit(0);
  //connect at API for upload video to wimtv
  $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
  $ch = curl_init();
  $url_upload = variable_get("basePathWimtv") . 'videos';

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
  $directory = "public://";
  $unique_temp_filename = $directory . "/" . time() . '.' . preg_replace('/.*?\//', '', "tmp");
  $unique_temp_filename = str_replace("\\" , "/" , $unique_temp_filename);
  if (!@move_uploaded_file( $urlfile , $unique_temp_filename)) {
    echo "non copiato";
  }

  $post= array("file" => "@" . drupal_realpath($unique_temp_filename),
  "title" => $titlefile,
  "description" => $descriptionfile
  );


  if (isset($video_category)) {
    $id=0;
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
    drupal_set_message( t("Upload Successfully") );
    $query = db_query("INSERT INTO {wimtvpro_videos} (uid,contentidentifier,mytimestamp,position,state, viewVideoModule,status,acquiredIdentifier,urlThumbs,category,title,duration,showtimeIdentifier) VALUES (
    '" . variable_get("userWimtv") . "','" . $arrayjsonst->contentIdentifier . "','" . time() . "',0,'','3','OWNED|" .  $file . "','','','','" . $titlefile . "','','')");

  }
  else
    form_set_error('' , t('Upload error'));
  */
    $file = @$_FILES['files']['name']['videoFile'];
    $sizefile = filesize($file);

    $error = "";

    if (empty($file)) {
        $error .= t('You must upload a file.');
    }
    if (empty($titlefile)) {
        $error .=  t('You must write a title.');
    }

    if ($error!="") {
        form_set_error('', check_plain($error));
        return false;
    }


    $directory = "public://";
    $unique_temp_filename = $directory . "/" . time() . '.' . preg_replace('/.*?\//', '', "tmp");
    $unique_temp_filename = str_replace("\\" , "/" , $unique_temp_filename);
    if (!@move_uploaded_file( $file , $unique_temp_filename)) {
        form_set_error('Upload', 'Cannot obtain access to file');
    }

    $error = 0;
    $titlefile = check_plain($_POST['titlefile']);
    $descriptionfile = check_plain($_POST['descriptionfile']);
    $video_category = check_plain($_POST['videoCategory']);
    $contentIdentifier = check_plain($_POST['uploadIdentifier']);

    if ((strlen(trim($file))>0) && ($error==0)) {
        //UPLOAD VIDEO INTO WIMTV
        set_time_limit(0);
        $post= array("file" => $unique_temp_filename,
              "title" => $titlefile,
              "description" => $descriptionfile,
              'uploadIdentifier' => $contentIdentifier);
        if (count($video_category)>0) {
            $id=0;
            foreach ($video_category as $cat) {
                $subcat = explode("|", $cat);
                if ($subcat[0]!=""){
                    $post['category[' . $id . ']'] = $subcat[0];
                    $post['subcategory[' . $id . ']'] = $subcat[1];
                    $id++;
                }
            }
        }

        $response = apiUpload($post);
        $arrayjsonst = json_decode($response);

        if (isset($arrayjsonst->contentIdentifier)) {
            drupal_set_message( t("Upload Successfully") );
            $query = db_query("INSERT INTO {wimtvpro_videos} (uid,contentidentifier,mytimestamp,position,state, viewVideoModule,status,acquiredIdentifier,urlThumbs,category,title,duration,showtimeIdentifier) VALUES (
    '" . variable_get("userWimtv") . "','" . $arrayjsonst->contentIdentifier . "','" . time() . "',0,'','3','OWNED|" .  $file . "','','','','" . $titlefile . "','','')");

        }
        else
            form_set_error('' , t('Upload error'));

    } else {
        $error++;
        form_set_error('' , t('Upload error'));
    }
}
