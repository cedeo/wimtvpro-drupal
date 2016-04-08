<?php

/**
 * Created with JetBrains PhpStorm.
 * User: walter
 * Date: 17/12/13
 * Time: 14.43
 */
function wimtvpro_wimbox() { 
    $view_page = wimtvpro_alert_reg();
    form_set_error("error", $view_page);

    if ($view_page == "") {
        $total_video_count = 0;
        $thumbs = wimtvpro_getThumbs(FALSE, TRUE, FALSE, "", FALSE, $total_video_count);
        return render_template('templates/wimbox.php', array('total_video_count' => $total_video_count, 'thumbs' => $thumbs));
    }
    return $view_page;
}

?>