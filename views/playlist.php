<?php

/**
 * Created with JetBrains PhpStorm.
 * User: walter
 * Date: 17/12/13
 * Time: 15.02
 */
function wimtvpro_playlist() {
    try {
        db_query("SELECT `option` FROM  {wimtvpro_playlist} ");
    } catch (Exception $e) {
        $sql = "ALTER IGNORE TABLE {wimtvpro_playlist} ADD `option` TEXT";
        db_query($sql);
    }

    $view_page = wimtvpro_alert_reg();
    form_set_error("error", $view_page);
    if ($view_page == "") {
        global $base_path;
        $urlCallAjax = url("admin/config/wimtvpro/wimtvproCallAjax");
        drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/css/wimtvpro.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
        drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/jquery/css/redmond/jquery-ui-1.8.21.custom.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
        drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/wimtvpro.js');

        $result = db_query("SELECT * FROM {wimtvpro_playlist} WHERE uid = '" . variable_get("userWimtv") . "' ORDER BY name ASC");
        $array_playlist = $result->fetchAll();
        $numberPlaylist = count($array_playlist);
        $count = 1;
        $playlists = array();

        if ($numberPlaylist > 0) {
            foreach ($array_playlist as $record) {
                $listVideo = $record->listVideo;
                $arrayVideo = explode(",", $listVideo);
                if (trim($listVideo) == "")
                    $record->countVideo = 0;
                else
                    $record->countVideo = count($arrayVideo);
                array_push($playlists, $record);
                $count++;
            }
        }

        $args = array('playlists' => $playlists,
            'count' => $count,
            'base_path' => $base_path,
            'urlCallAjax' => $urlCallAjax);
        return render_template('templates/playlist.php', $args);
    }

    return $view_page;
}

?>