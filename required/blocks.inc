<?php
/**
  * @file
  * This file is use for configured the Drupal's block.
 *
 */

function wimtvpro_block_configure($delta = '') {
    return array();
}

function wimtvpro_blockUser_config() {
    $view_page = wimtvpro_alert_reg();
    if ($view_page)
        form_set_error("error",$view_page);

    $params = array("ImageLogoProfile", "pageNameProfile", "personaleDateProfile", "EmailProfile", "SocialProfile");
    foreach ($params as $variable)
        if (isset($_POST[$variable]))
            variable_set($variable, $_POST[$variable]);

    if ($view_page==""){
        $desc = "Would you like view ## into block user profile?";
        $form['ImageLogoProfile'] = array(
            '#title' => "Image Logo",
            '#description' => str_replace("##", "Image Logo", $desc),
            '#type' => 'checkbox',
            '#return_value' => 1,
            '#default_value' => variable_get('ImageLogoProfile', 1),
        );
        $form['pageNameProfile'] = array(
            '#title' => "Page name",
            '#description' => str_replace("##", "page name", $desc),
            '#type' => 'checkbox',
            '#return_value' => 1,
            '#default_value' => variable_get('pageNameProfile', 1),
        );
        $form['personalDateProfile'] = array(
            '#title' => "Personal Date",
            '#description' => str_replace("##", "date of birth, sex,  name and surname", $desc),
            '#type' => 'checkbox',
            '#return_value' => 1,
            '#default_value' => variable_get('personalDateProfile', 0),
        );
        $form['EmailProfile'] = array(
            '#title' => "Email",
            '#description' => str_replace("##", "email", $desc),
            '#type' => 'checkbox',
            '#return_value' => 1,
            '#default_value' => variable_get('EmailProfile', 0),
        );
        $form['SocialProfile'] = array(
            '#title' => "Social ",
            '#description' => str_replace("##", "link to LinkedIn, Facebook, Twitter", $desc),
            '#type' => 'checkbox',
            '#return_value' => 1,
            '#default_value' => variable_get('SocialProfile', 1),
        );
        return system_settings_form($form);
    }
}

//Info block
function wimtvpro_block_info() {
    // This example comes from node.module.
    $blocks['user_profile'] = array( 'info' => t('Block User Profile'));
    $blocks['video_thumbs'] = array('info' => t('Block list video WimVod'));
    return $blocks;
}

function wimtvpro_block_view($delta = '') {
    // This example is adapted from node.module.
    global  $base_url;
    $block = array();
    switch ($delta) {
        case 'user_profile':
            $block['subject'] = "My WimTv Profile";
            //request by api info user
            $response = apiGetProfile();
            $arrayjsuser = json_decode($response);
            $profileuser= "";
            $namepage = "";

            if (variable_get("ImageLogoProfile")=="1")
                $profileuser .= "<img src='" . $arrayjsuser ->imageLogoPath . "'>";

            if (variable_get("pageNameProfile")=="1")
                if (isset($arrayjsuser->pageName))
                    $namepage .= "<p><b>" . $arrayjsuser->pageName . "</b><br/>" . $arrayjsuser->pageDescription . "</p>";
                else
                    $namepage .= "<p><b>" . $arrayjsuser->username . "</b></p>";
            $profileuser .= $namepage;

            if (variable_get("personalDateProfile")=="1")
                $profileuser .= "<p><b>" . t("My Detail") . "</b><br/>" .
            $arrayjsuser->name . " " . $arrayjsuser->surname . "<br/>" .
            $arrayjsuser->dateOfBirth . "<br/>" . $arrayjsuser->sex . "<br/>" . "</p>";

            if (variable_get("EmailProfile")=="1")
                $profileuser .= "<p><b>" . t("Contact") . "</b><br/>" . $arrayjsuser->email . "<br/>";

            if (variable_get("SocialProfile")=="1") {
              if (isset($arrayjsuser->linkedinURI))
                $profileuser .= "<a target='_new' href='" . $arrayjsuser->linkedinURI . "'><img src='" . $base_url . "/" . drupal_get_path('module', 'wimtvpro') . "/img/linkedin.png'></a>";
              if (isset($arrayjsuser->twitterURI))
                $profileuser .= "<a target='_new' href='" . $arrayjsuser->twitterURI . "'><img src='" . $base_url . "/" . drupal_get_path('module', 'wimtvpro') . "/img/twitter.png'></a>";
              if (isset($arrayjsuser->facebookURI))
                $profileuser .= "<a target='_new' href='" . $arrayjsuser->facebookURI . "'><img src='" . $base_url . "/" . drupal_get_path('module', 'wimtvpro') . "/img/facebook.png'></a>";
              $profileuser .= "</p>";
            }
            $block['content'] = $profileuser;

            break;


        case 'video_thumbs':
            $block['subject'] = "My Videos";
            $block['content']= "<ul class='itemsPublic'>" . wimtvpro_getThumbs(TRUE, FALSE, FALSE, "block") . "</ul><div class='empty'></div>";

            break;

    }
    return $block;
}

function wimtvpro_public() {
    return "<ul class='itemsPublic'>" . wimtvpro_getThumbs(TRUE, FALSE, FALSE, "page") . "</ul>";
}

