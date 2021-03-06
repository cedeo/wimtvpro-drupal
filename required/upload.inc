<?php

/**
 * @file
 * This file is use for configured form for upload new video.
 * NS: FUNCTIONS TO MANAGE WIMBOX UPLOAD NEW VIDEO PAGE
 * NOTE: THIS IS NOT USED BY WIMTV VIDEO CONTENT FIELD (EX WIMTV PRO PLUS)
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
    form_set_error("error", $view_page);

    $form = array('#attributes' => array('enctype' => 'multipart/form-data'));

    if ($view_page == "") {

//        $form['htmltag_wimboxlink'] = array(
//            '#markup' => l(t('WimBox'), 'admin/config/wimtvpro/wimbox'),
//            '#weight' => 0,
//        );

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


        $url_categories = cms_getWimtvApiUrl() . "videoCategories";
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
            '#markup' => variable_get('htmltag2', "<p class='description' id='addCategories'></p>"),
            '#weight' => 5,
        );


        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Upload'),
            '#weight' => 6,
        );

//        $form['redirect'] = "/admin/config/wimtvpro/wimbox";//drupal_get_path('module', 'wimtvpro');

        $form['#validate'][] = 'wimtvpro_upload_validate';

        // NS: WE HAVE TO VERIFY THE FOLLOWING LINE!
        $form['#submit'][] = 'wimtvpro_upload_submit';

        $form['htmltag'] = array(
            '#markup' => variable_get('htmltag', "<div class='action'><span class='icon_sync2' style='display:none;'>Loading...</span>
" . t("Do not leave this page until the file upload is not terminated") .
                    "</div>"),
            '#weight' => 7,
        );
    }
    return $form;
}

// Validate for video upload
function wimtvpro_upload_validate($form, &$form_state) {
    $query = NULL;
    $file = $_FILES['files']['name']['videoFile'];
    $urlfile = $_FILES['files']['tmp_name']["videoFile"];
    $titlefile = check_plain($_POST['titlefile']);

    $error = "";

    if (empty($file)) {
        $error .= t('Please upload a file');
    }
    if (empty($titlefile)) {
        $error .= t('Please add title');
    }

    if ($error != "") {
        form_set_error('', check_plain($error));
        return false;
    }


    $directory = "public://";
    $unique_temp_filename = $directory . "/" . time() . '.' . preg_replace('/.*?\//', '', "tmp");
    $unique_temp_filename = str_replace("\\", "/", $unique_temp_filename);
    $moved = @move_uploaded_file($urlfile, $unique_temp_filename);
    if (!$moved) {
        form_set_error('Upload', t('Cannot obtain access to file'));
        $error++;
    }

    $descriptionfile = check_plain($_POST['descriptionfile']);

    // NS: $_POST['videoCategory'] is an array: we cannot call the "check_plain"
    // method on it!
    // $video_category = isset($_POST['videoCategory']) ? check_plain($_POST['videoCategory']) : "";
    // $video_category = explode(",", $video_category);

    $video_category = "";
    if (isset($_POST['videoCategory']) && is_array($_POST['videoCategory'])) {
        foreach ($_POST['videoCategory'] as $key => $value) {
            $video_category[$key] = check_plain($value);
        }
    }


    $contentIdentifier = isset($_POST['uploadIdentifier']) ? check_plain($_POST['uploadIdentifier']) : "";

    if ((strlen(trim($file)) > 0) && ($error == 0)) {
        set_time_limit(0);
        $post = array(
            "file" => drupal_realpath($unique_temp_filename),
            "title" => $titlefile,
            "description" => $descriptionfile,
            'uploadIdentifier' => $contentIdentifier);
        if (count($video_category) > 0) {
            $id = 0;
            foreach ($video_category as $cat) {
                $subcat = explode("|", $cat);
                if ($subcat[0] != "") {
                    $post['category[' . $id . ']'] = $subcat[0];
                    $post['subcategory[' . $id . ']'] = $subcat[1];
                    $id++;
                }
            }
        }

        $response = apiUpload($post);
        $arrayjsonst = json_decode($response);

        if (isset($arrayjsonst->contentIdentifier)) {
//            drupal_set_message(t("Upload successful") . " - " . l(t('Back to WimBox'), 'admin/config/wimtvpro/wimbox'));
            drupal_set_message(t("Upload successful") . " - " . l(t('Back to WimBox'), 'admin/config/' . getWhiteLabel('APP_NAME') . '/' . getWhiteLabel('WIMBOX_urlLink')));
            // NS: We use the "check_plain" method to safely insert strange 
            // filename chars into database.
//            $query = db_query("INSERT INTO {wimtvpro_videos} (uid,contentidentifier,mytimestamp,position,state, viewVideoModule,status,acquiredIdentifier,urlThumbs,category,title,duration,showtimeIdentifier) VALUES (
//    '" . variable_get("userWimtv") . "','" . $arrayjsonst->contentIdentifier . "','" . time() . "',0,'','3','OWNED|" . $file . "','','','','" . $titlefile . "','','')");
            $plain_filename = check_plain($file);
            $query = "INSERT INTO {wimtvpro_videos} (uid,contentidentifier,mytimestamp,position,state, viewVideoModule,status,acquiredIdentifier,urlThumbs,category,title,duration,showtimeIdentifier) VALUES (
    '" . variable_get("userWimtv") . "','" . $arrayjsonst->contentIdentifier . "','" . time() . "',0,'','3','OWNED|" . $plain_filename . "','','','','" . $titlefile . "','','')";
            watchdog("wimtv-upload video", "successfully uploaded file" . $plain_filename);
            $res = db_query($query);
        } else {
            form_set_error('', t('Upload error'));
        }
    } else {
        $error++;
        form_set_error('', t('Upload error'));
    }
}
