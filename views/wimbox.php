<?php
/**
 * Created with JetBrains PhpStorm.
 * User: walter
 * Date: 17/12/13
 * Time: 14.43
 */
/**
 * Gestisce la sezione wimbox del plugin
 */
function wimtvpro_wimbox() {
    $view_page = wimtvpro_alert_reg();
    form_set_error("error", $view_page);

    if ($view_page==""){
        return render_template('templates/wimbox.php', array('thumbs' => wimtvpro_getThumbs(FALSE)));
    }
    return $view_page;
}