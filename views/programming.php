<?php

/**
 * NS: Logica che recupera la lista dei palinsesti.
 */
//require_once(drupal_get_path('module', 'wimtvpro') . '/api/wimtv_api.php');

//function initProgrammings() {
//    //PROGRAMMING SCRIPTS
//    // Set js variables
//    $baseWimTvRoot = "https://www.wim.tv/wimtv-webapp";
//    drupal_add_js("var url_pathPlugin='/" . drupal_get_path('module', 'wimtvpro') . "/'", 'inline');
//    drupal_add_js("var imageBase='" . $baseWimTvRoot . "'", 'inline');
//
//    // Load CSS
//    $options = array("type" => "external");
//    drupal_add_css($baseWimTvRoot . '/css/fullcalendar.css', $options);
//    drupal_add_css($baseWimTvRoot . '/css/programming.css', $options);
//    drupal_add_css($baseWimTvRoot . '/css/jquery-ui/jquery-ui.custom.min.css', $options);
//    drupal_add_css($baseWimTvRoot . '/css/jquery.fancybox.css', $options);
//    drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/css/programming.css');
//    drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/css/wimtvpro_public.css');
//
//    // Load JS Script
//    drupal_add_js('https://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js');
////    drupal_add_js('https://ajax.googleapis.com/ajax/libs/jquery/1.4.4/jquery.min.js');    
//    drupal_add_js($baseWimTvRoot . '/script/jquery-ui.custom.min.js');
//    drupal_add_js($baseWimTvRoot . '/script/jquery.fancybox.min.js');
//    drupal_add_js($baseWimTvRoot . '/script/jquery.mousewheel.min.js');
//    drupal_add_js($baseWimTvRoot . '/script/fullcalendar/fullcalendar.min.js');
//    drupal_add_js($baseWimTvRoot . '/script/utils.js');
//    drupal_add_js($baseWimTvRoot . '/script/programming/programming.js');
//    drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/programming-api.js');
//    drupal_add_js($baseWimTvRoot . '/script/programming/calendar.js');
//}

function wimtvpro_programmingList() {
    $view_page = wimtvpro_alert_reg();
    form_set_error("error", $view_page);

    if ($view_page == "") {
        $response = apiGetProgrammings();
        $arrayjsonst = json_decode($response);
        return render_template('templates/programming/programmingList.php', array('arrayjsonst' => $arrayjsonst));
    }
    return $view_page;
}

function wimtvpro_programming_add() {
    return wimtvpro_render_programming_iframe();
//    initProgrammings();
//    $view_page = wimtvpro_alert_reg();
//    form_set_error("error", $view_page);
//
//    if ($view_page == "") {
//        $template = 'templates/programming/programmingAdd.php';
//        return render_template($template);
//    }
//    return $view_page;
}

function wimtvpro_programming_edit() {
    return wimtvpro_render_programming_iframe();
//    initProgrammings();
//    $view_page = wimtvpro_alert_reg();
//    form_set_error("error", $view_page);
//
//    if ($view_page == "") {
//        $template = 'templates/programming/programmingEdit.php';
//        return render_template($template);
//    }
//    return $view_page;
}

function wimtvpro_render_programming_iframe() {
    $view_page = wimtvpro_alert_reg();
    form_set_error("error", $view_page);

    if ($view_page == "") {
        $template = 'templates/programming/programmingTemplate.php';
        return render_template($template);
    }
    return $view_page;
}

function wimtvpro_programming_delete() {
//    initProgrammings();
    $view_page = wimtvpro_alert_reg();
    form_set_error("error", $view_page);
//    watchdog("wimtv_programmings_DELETE", implode(",", $_GET));
    if ($view_page == "") {
        $progId = isset($_GET["progId"]) ? $_GET["progId"] : null;
//        watchdog("wimtv_programmings_DELETE", $progId);
//        var_dump("DELETING: ".$progId);
        if ($progId !== null) {
            apiDeleteProgramming($progId);
        }
//        drupal_goto("/admin/config/wimtvpro/programming/");
        drupal_goto('/admin/config/' . getWhiteLabel('APP_NAME') . '/' . getWhiteLabel('SCHEDULES_urlLink'));
    }
    return $view_page;
}
?>

