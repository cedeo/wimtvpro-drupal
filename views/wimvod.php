<?php
/**
 * Created with JetBrains PhpStorm.
 * User: walter
 * Date: 17/12/13
 * Time: 15.02
 */
/**
 * Gestisce la sezione wimvod del plugin
 */
function wimtvpro_wimvod() {
    $view_page = wimtvpro_alert_reg();
    drupal_add_js('
    //Request new URL for create a wimlive Url
    jQuery(document).ready(function(){

        jQuery(".icon_download").click(function() {
            var id = jQuery(this).attr("id").split("|");

            var uri = "' . variable_get("basePathWimtv") . 'videos/" + id[0] + "/download";
            if (id[1]!=""){
                var file = id[1].split(".");
                uri = uri + "?ext=" + file[1] + "&filename=" + file[0];
            }
            jQuery("body").append("<iframe src=\"" + uri + "\" style=\"display:none;\" />");

        });
    });  ','inline');
    form_set_error("error",$view_page);
    if ($view_page==""){
        return render_template('templates/wimvod.php', array('thumbs' => wimtvpro_getThumbs(TRUE)));
    }
    return $view_page;
}