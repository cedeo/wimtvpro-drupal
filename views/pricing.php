<?php
/**
 * Created by JetBrains PhpStorm.
 * User: walter
 * Date: 17/12/13
 * Time: 12.02
 */
/**
 * Gestisce la sezione pricing del plugin
 */
function wimtvpro_callPricing() {
    $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");

    if ($credential!="username:password") {

        $directoryCookie = "public://cookieWim";
        $script = "";
        // If directory cookieWim don't exist, create the directory (if change Public file system path into admin/config/media/file-system after installation of this module or is the first time)
        if (!is_dir($directoryCookie)) {
            $directory_create = drupal_mkdir('public://cookieWim',777);
        }

        if (isset($_GET['upgrade'])){
            drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/colorbox/js/jquery.colorbox.js');
            drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/jquery/colorbox/css/colorbox.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
            $fileCookie = "cookies_" . variable_get("userWimtv") . "_" . $_GET['upgrade'] . ".txt";

            if (!is_file($directoryCookie. "/" . $fileCookie)) {
                $f = fopen($directoryCookie. "/" . $fileCookie,"w");
                fwrite($f,"");
                fclose($f);
            }
            //Update Packet
            $data = array("name" => $_GET['upgrade']);
            $data_string = json_encode($data);

            global $base_url;
            $my_page =  url($base_url . "/admin/config/wimtvpro?pack=1&success=" . $_GET['upgrade']);
            $result = apiUpgradePacket(urlencode($my_page), $fileCookie, $data_string);
            $arrayjsonst = json_decode($result);

            if ($arrayjsonst->result=="REDIRECT") {
                $script = "
                 <script>
                      jQuery(document).ready(function() {
                        jQuery.colorbox({
                            onLoad: function() {
                                jQuery('#cboxClose').remove();
                            },
                            html:'<h2>" . $arrayjsonst->message . "</h2><h2><a href=\"" . $arrayjsonst->successUrl . "\">Yes</a> | <a onClick=\"jQuery(this).colorbox.close();\" href=\"#\">" . t("No") . "</a></h2>'
                        })
                     });
                 </script>
                  ";
            } else {
                $script = "
                 <script>
                      jQuery(document).ready(function() {
                        jQuery.colorbox({
                            html:'" . $arrayjsonst->html . "'
                        })
                     });
                 </script>
                  ";
            }

        }

        if (isset($_GET['success'])) {
            $fileCookie = "cookies_" . variable_get("userWimtv") . "_" . $_GET['success'] . ".txt";
            $result = apiCheckPayment($fileCookie);
            $arrayjsonst = json_decode($result);
        }


        $response = apiGetPacket();
        $packet_user_json = json_decode($response);

        $id_packet_user = isset($packet_user_json->id) ? $packet_user_json->id : "";
        $createDate_packet_user = isset($packet_user_json->createDate) ? $packet_user_json->createDate : "";
        $updateDate_packet_user = isset($packet_user_json->updateDate) ? $packet_user_json->updateDate : "";

        $createDate = date('d/m/Y', $createDate_packet_user/1000);
        $updateDate = date('d/m/Y', $updateDate_packet_user/1000);
        $dateRange = getDateRange($createDate , $updateDate );

        $count_date = isset($packet_user_json->daysLeft) ? $packet_user_json->daysLeft : "";
        $response2 = apiCommercialPacket();

        $packet_json = json_decode($response2);
        $args = array('packet_json' => $packet_json,
            'count_date' => $count_date,
            'id_packet_user' => $id_packet_user,
            'script' => $script);
        return render_template('templates/pricing.php', $args);
    }
    return "";
}