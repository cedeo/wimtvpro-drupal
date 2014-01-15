<?php
/**
 * Created by JetBrains PhpStorm.
 * User: walter
 * Date: 17/12/13
 * Time: 14.22
 * To change this template use File | Settings | File Templates.
 */
function wimtvpro_admin() {

    $view_page = wimtvpro_alert_reg();

    drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/wimtvpro.js');
    $response = apiGetProfile();
    $dati = json_decode($response, true);

    $form = array();
    $form ['#attributes'] = array("enctype" => "multipart/form-data");
    if (variable_get("sandbox")=="yes") {
        $form['htmltag'] = array(
            '#markup' => variable_get('htmltag',
                t(""))
        );
        $form['basePathWimtv'] = array(
            '#type' => 'hidden',
            '#value' => 'http://peer.wim.tv/wimtv-webapp/rest/',
        );
    }
    else {
        $form['htmltag'] = array(
            '#markup' => variable_get('htmltag',
                t(""))
        );
        $form['basePathWimtv'] = array(
            '#type' => 'hidden',
            '#value' => 'https://www.wim.tv:443/wimtv-webapp/rest/',
        );
    }

    $form['fieldConfig'] = array('#type'=>'fieldset','#title'=>'Configuration','#collapsible' => TRUE, '#collapsed' => FALSE);

    //FieldsetConfig
    $form['fieldConfig']['userWimtv'] = array(
        '#type' => 'textfield',
        '#title' => t('Username Wimtv'),
        '#default_value' => variable_get('userWimtv', 'username'),
        '#size' => 100,
        '#maxlength' => 200,
        '#required' => TRUE,
    );

    $form['fieldConfig']['passWimtv'] = array(
        '#type' => 'password',
        '#title' => t('Password Wimtv'),
        '#default_value' => variable_get('passWimtv', 'password'),
        '#size' => 100,
        '#maxlength' => 200,
        '#required' => TRUE,
        '#attributes' => array('value' => variable_get('passWimtv', 'password')),
    );

    $form['fieldConfig']['htmltag2'] = array(
        '#markup' => variable_get('htmltag2',
            t("<p>-> Upload and/or choose the skin player into <a target='new' href='http://www.longtailvideo.com/addons/skins'>page Jwplayer</a> for your videos</p>"))

    );

    //Read directory for skin JWPLAYER
    $elencoSkin = array();
    $directory = "public://skinWim";
    // If directory skinWim don't exist, create the directory (if change Public file system path into admin/config/media/file-system after installation of this module or is the first time)
    if (!is_dir($directory)) {
        $directory_create = drupal_mkdir('public://skinWim');
    }

    $elencoSkin[""] = t("-- Base Skin --");
    if (is_dir($directory)) {
        if ($directory_handle = opendir($directory)) {
            while (($file = readdir($directory_handle)) !== FALSE) {
                if ((!is_dir($file)) && ($file!=".") && ($file!="..")) {
                    $explodeFile = explode("." , $file);
                    if ($explodeFile[1]=="zip")
                        $elencoSkin[$explodeFile[0]] = $explodeFile[0];
                }
            }
            closedir($directory_handle);
        }
    }

    $form['fieldConfig']['nameSkin'] = array(
        '#type' => 'select',
        '#title' => t('Name Skin'),
        '#options' => $elencoSkin,
        '#default_value' => variable_get('nameSkin'),
        '#required' => FALSE,
    );

    $form['fieldConfig']['uploadSkin'] = array(
        '#type' => 'file',
        '#title' => t('Upload new skin player'),
        '#size' => 100,
        '#maxlength' => 200,
        '#required' => FALSE,
        '#description' => t('Only zip. Save into a public URL&nbsp;') . file_create_url($directory) . t('<br/>For running the skin selected, copy the file <a href="http://plugins.longtailvideo.com/crossdomain.xml" target="_new">crossdomain.xml</a> to the root directory (e.g. http://www.mysite.it). You can do it all from your FTP program (e.g. FileZila, Classic FTP, etc).
So open up your FTP client program. First, identify your root directory. This is the folder titled or beginning with "www" -- and this is where you ultimately need to move that pesky crossdomain.xml. Now all you have to do is find it.'),
    );


    $form['fieldConfig']['htmltag3'] = array(
        '#markup' => variable_get('htmltag3',
            t("<p>-> Dimensions of player for your videos</p>"))

    );


    $form['fieldConfig']['heightPreview'] = array(
        '#type' => 'textfield',
        '#title' => t('Height (default: 280)'),
        '#default_value' => variable_get('heightPreview', '280'),
        '#size' => 100,
        '#maxlength' => 200,
        '#required' => FALSE,
    );
    $form['fieldConfig']['widthPreview'] = array(
        '#type' => 'textfield',
        '#title' => t('Width (default: 500)'),
        '#default_value' => variable_get('widthPreview', '500'),
        '#size' => 100,
        '#maxlength' => 200,
        '#required' => FALSE,
    );
    /*
    $form['fieldConfig']['sandbox'] = array(
    '#title' => t('Please select "no" to use WimTVPro plugin on WimTV server. Select "yes" if you want to to try the service on test server'),
    '#type' => 'select',
    '#maxlength' => 5,
    '#options' => array( 'no' => 'No', 'yes' => 'Yes, for developer and tester'),
    '#required' => TRUE,
    '#default_value' => variable_get('sandbox', 'no'),
    );
    */
    $form['fieldConfig']['sandbox'] = array(
        '#type' => 'hidden',
        '#value' => t('no')
    );
    $form['fieldConfig']['addPageMyStreaming'] = array(
        '#title' => t('Would you like to add a public (visible to End Users) MyStreaming page to your web site?'),
        '#type' => 'select',
        '#maxlength' => 5,
        '#options' => array( 'no' => 'No', 'yes' => 'Yes, add a page My WimTv Streaming'),
        '#required' => TRUE,
        '#default_value' => variable_get('addPageMyStreaming', 'no'),
    );
    $my_fields = field_info_fields();

    if (count($my_fields)>0) {

        foreach ($my_fields as $key => $value) {
            if (($my_fields[$key]["type"] == "text_with_summary") || ($my_fields[$key]["type"] == "text_long"))
                $content[$key]=$key;
        }

        $form['fieldConfig']['contentItemIntoInsert'] = array(
            '#title' => t('Select fields where you want to add the video'),
            '#type' => 'checkboxes',
            '#options' => $content,
        );
        if (count(variable_get('contenttypeWithInsertVideo'))) {
            $form['contentItemIntoInsert']['#default_value'] = variable_get('contenttypeWithInsertVideo');
        }
        $form['contentItemIntoInsert']['comment_body']['#attributes']['selected'] = TRUE;
    }
    //End FieldsetConfig

    if ($view_page==""){
        $openFieldSet = FALSE;
        if ($openFieldSet)
            $form['fieldPricing'] = array('#type'=>'fieldset','#title'=>'Pricing','#collapsible' => TRUE, '#collapsed' => FALSE);
        else
            $form['fieldPricing'] = array('#type'=>'fieldset','#title'=>'Pricing','#collapsible' => TRUE, '#collapsed' => TRUE);
        $form['fieldPayment'] = array('#type'=>'fieldset','#title'=>'Payment','#collapsible' => TRUE, '#collapsed' => TRUE);
        $form['fieldLive'] = array('#type'=>'fieldset','#title'=>'WimLive Configuration','#collapsible' => TRUE, '#collapsed' => TRUE);
        //$form['fieldPersonal'] = array('#type'=>'fieldset','#title'=>'Update Personal Info','#collapsible' => TRUE, '#collapsed' => TRUE);
        $form['fieldFeatures'] = array('#type'=>'fieldset','#title'=>'Features','#collapsible' => TRUE, '#collapsed' => TRUE);

        //fieldPricing
        $pricing = wimtvpro_callPricing();
        $form['fieldPricing']['htmlFrame'] = array('#markup' => variable_get('htmltag3',$pricing) );
        //End fieldPricing

        //fieldPayment
        $form['fieldPayment']['paypalEmail'] = array('#type' => 'textfield',
            '#title' => t('Paypal Email'),
            '#default_value' => !empty($dati['paypalEmail']) ? $dati['paypalEmail'] : '',
            '#size' => 100,
            '#maxlength' => 200,
            '#required' => FALSE,);
        $form['fieldPayment']['taxCode'] = array('#type' => 'textfield',
            '#title' => t('Tax Code'),
            '#default_value' => !empty($dati['taxCode']) ? $dati['taxCode'] : '',
            '#size' => 100,
            '#maxlength' => 200,
            '#required' => FALSE,);
        $form['fieldPayment']['vatCode'] = array('#type' => 'textfield',
            '#title' => t('Vat Code'),
            '#default_value' => !empty($dati['vatCode']) ? $dati['vatCode'] : '',
            '#size' => 100,
            '#maxlength' => 200,
            '#required' => FALSE,);
        $form['fieldPayment']['htmlBilling']=array('#markup' => variable_get('htmltag3',t("Billing Adress")) );
        $form['fieldPayment']['billingAddress[street]'] = array('#type' => 'textfield',
            '#title' => t('Street'),
            '#default_value' => !empty($dati['billingAddress']['street']) ? $dati['billingAddress']['street'] : '',
            '#size' => 100,
            '#maxlength' => 200,
            '#required' => FALSE,);
        $form['fieldPayment']['billingAddress[city]'] = array('#type' => 'textfield',
            '#title' => t('Street'),
            '#default_value' => !empty($dati['billingAddress']['city']) ? $dati['billingAddress']['city'] : '',
            '#size' => 100,
            '#maxlength' => 200,
            '#required' => FALSE,);
        $form['fieldPayment']['billingAddress[state]'] = array('#type' => 'textfield',
            '#title' => t('State'),
            '#default_value' => !empty($dati['billingAddress']['state']) ? $dati['billingAddress']['state'] : '',
            '#size' => 100,
            '#maxlength' => 200,
            '#required' => FALSE,);
        $form['fieldPayment']['billingAddress[zipCode]'] = array('#type' => 'textfield',
            '#title' => t('Zip Code'),
            '#default_value' => !empty($dati['billingAddress']['zipCode']) ? $dati['billingAddress']['zipCode'] : '',
            '#size' => 100,
            '#maxlength' => 200,
            '#required' => FALSE,);
        //End fieldPayment

        //fieldLive
        $form['fieldLive']['html']=  array('#markup' => variable_get('htmltag3', t('<p>In this section you can enable the more functional live streaming settings for your needs. Choose between "Live streaming" to stream your own events, or use the features reserved for event Resellers and event Organizers to sell and organize live events.</p>')) );
        $form['fieldLive']['liveStreamEnabled'] = array('#type' => 'checkbox',
            '#title' => t('Live streaming'),
            '#default_value' => !empty($dati['liveStreamEnabled']) ? $dati['liveStreamEnabled'] : '',
            '#return_value' => 'true',
            '#description' => t('Enables the feature that allows you to broadcast your live streaming events with WimTV.'),
            '#required' => FALSE,);
        $form['fieldLive']['liveStreamPwd'] = array('#type' => 'password',
            '#title' => t('Live stream events resale'),
            '#default_value' => !isset($dati['liveStreamPwd']) ? $dati['liveStreamPwd'] : "",
            '#description' => t('This password is required for the live streaming'),
            '#required' => FALSE,);
        $form['fieldLive']['eventResellerEnabled'] = array('#type' => 'checkbox',
            '#title' => t('Live stream events resale'),
            '#default_value' => !empty($dati['eventResellerEnabled']) ? $dati['eventResellerEnabled'] : '',
            '#return_value' => 'true',
            '#description' => t('Enables you to resell the streaming of live events organized bu other Web TVs.'),
            '#required' => FALSE,);
        $form['fieldLive']['eventOrganizerEnabled'] = array('#type' => 'checkbox',
            '#title' => t('Live stream events organizing'),
            '#default_value' => !empty($dati['eventOrganizerEnabled']) ? $dati['eventOrganizerEnabled'] : '',
            '#return_value' => 'true',
            '#description' => t('Enables the feature that allows you to organize live streaming events.'),
            '#required' => FALSE,);
        //End fieldLive

        //fieldFeatures
        $form['fieldFeatures']['hidePublicShowtimeVideos'] = array('#type' => 'select',
            '#title' => t('Index and show public videos into WimTv\'s site'),
            '#options' => array( "true" =>" No" , "false" => "Si"),
            '#default_value' => $dati['hidePublicShowtimeVideos'],
            '#required' => FALSE,);
        //End fieldFeatures
    }

    $form['urlVideosWimtv'] = array(
        '#type' => 'hidden',
        '#value' => 'videos',
    );
    $form['urlVideosDetailWimtv'] = array(
        '#type' => 'hidden',
        '#value' => 'videos?details=true&incomplete=true',
    );
    $form['urlThumbsWimtv'] = array(
        '#type' => 'hidden',
        '#value' => 'videos/{contentIdentifier}/thumbnail',
    );
    $form['urlEmbeddedPlayerWimtv'] = array(
        '#type' => 'hidden',
        '#value' => 'videos/{contentIdentifier}/embeddedPlayers?get=1',
    );
    $form['urlPostPublicWimtv'] = array(
        '#type' => 'hidden',
        '#value' => 'videos/{contentIdentifier}/showtime',
    );
    $form['urlPostPublicAcquiWimtv'] = array(
        '#type' => 'hidden',
        '#value' => 'videos/{contentIdentifier}/acquired/{acquiredIdentifier}/showtime',
    );
    $form['urlSTWimtv'] = array(
        '#type' => 'hidden',
        '#value' => 'videos/{contentIdentifier}/showtime/{showtimeIdentifier}',
    );
    $form['urlShowTimeWimtv'] = array(
        '#type' => 'hidden',
        '#value' => 'users/{username}/showtime',
    );
    $form['urlShowTimeDetailWimtv'] = array(
        '#type' => 'hidden',
        '#value' => 'users/{username}/showtime?details=true',
    );
    $form['urlUserProfileWimtv'] = array(
        '#type' => 'hidden',
        '#value' => 'users/{username}/profile',
    );
    $form['replaceContentWimtv'] = array(
        '#type' => 'hidden',
        '#value' => '{contentIdentifier}',
    );
    $form['replaceUserWimtv'] = array(
        '#type' => 'hidden',
        '#value' => '{username}',
    );
    $form['replaceacquiredIdentifier'] = array(
        '#type' => 'hidden',
        '#value' => '{acquiredIdentifier}',
    );
    $form['replaceshowtimeIdentifier'] = array(
        '#type' => 'hidden',
        '#value' => '{showtimeIdentifier}',
    );
    unset($form['addPageMyStreaming']);

    $form['#validate'][] = 'wimtvpro_admin_validate';
    return system_settings_form($form);
}

function wimtvpro_admin_validate($form, &$form_state) {
    $view_page = wimtvpro_alert_reg();
    $file = $_FILES['files']['name']["uploadSkin"];
    $directory = "public://skinWim";
    $arrayFile = explode(".", $file);
    if (!empty($file)) {
        if ($arrayFile[1] != "zip")
            form_set_error("", t("This file isn't format correct for jwplayer's skin"));
        else {
            $validators = array(
                'file_validate_extensions' => array('zip')
            );
            file_save_upload("uploadSkin", $validators, $directory);
            form_set_value("Upload", $arrayFile[0], $form_state);
        }
    }

    if (isset($_POST["addPageMyStreaming"]) && ($_POST["addPageMyStreaming"]=="yes")) {
        $query = db_update('{menu_links}')
            ->fields(array(
                'hidden' => "0",
            )) -> condition("link_path", "wimtvpro")
            ->execute();
    }
    else {
        $query = db_update('{menu_links}')
            ->fields(array(
                'hidden' => "-1",
            )) -> condition("link_path", "wimtvpro")
            ->execute();
    }
    menu_rebuild();

    //read check content type where use for insert video
    if (isset($_POST["contentItemIntoInsert"]))
        variable_set("contenttypeWithInsertVideo", $_POST["contentItemIntoInsert"]);
    else
        variable_set("contenttypeWithInsertVideo", array());

    variable_set('heightPreview', $_POST['heightPreview']);
    variable_set('widthPreview', $_POST['widthPreview']);

    //echo variable_get('heightPreview');
    //echo variable_get('widthPreview');
    //fieldConfig
    //fieldPricing
    //fieldPayment
    //fieldLive
    //fieldFeatures

    if ($view_page=="") {
        $dati= array();
        $dati["paypalEmail"]= $_POST["paypalEmail"];
        $dati["taxCode"]=  $_POST["taxCode"];
        $dati["vatCode"]=  $_POST["vatCode"];
        $dati["billingAddress"]["street"]=  $_POST["billingAddress"]["street"];
        $dati["billingAddress"]["city"]=  $_POST["billingAddress"]["city"];
        $dati["billingAddress"]["state"]=  $_POST["billingAddress"]["state"];
        $dati["billingAddress"]["zipCode"]=  $_POST["billingAddress"]["zipCode"];
        $dati["liveStreamEnabled"]= isset($_POST["liveStreamEnabled"]) ? 'true' : 'false';
        $dati["eventResellerEnabled"]= isset($_POST["eventResellerEnabled"]) ? 'true' : 'false';
        $dati["eventOrganizerEnabled"]= isset($_POST["eventOrganizerEnabled"]) ? 'true' : 'false';
        $dati["hidePublicShowtimeVideos"]= $_POST["hidePublicShowtimeVideos"];
        $dati["liveStreamPwd"]=  $_POST["liveStreamPwd"];

        if (count($dati)>0){
            $response = apiEditProfile($dati);
            $arrayjsonst = json_decode($response);
            if (isset($arrayjsonst->result) && ($arrayjsonst->result!="SUCCESS")) {
                $testoErrore = "";
                foreach ($arrayjsonst->messages as $message){
                    $testoErrore .=  $message->field . " : " .  $message->message . "<br/>";
                }
                form_set_error("Errore Curl", $testoErrore);
            }
        }
    }
}
