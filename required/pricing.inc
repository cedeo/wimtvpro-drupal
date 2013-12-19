<?php
/**
 * Created by JetBrains PhpStorm.
 * User: walter
 * Date: 17/12/13
 * Time: 12.02
 * To change this template use File | Settings | File Templates.
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

            // chiama
            $ch = curl_init();

            global $base_url;
            $my_page =  url($base_url . "/admin/config/wimtvpro?pack=1&success=" . $_GET['upgrade']);
            curl_setopt($ch, CURLOPT_URL,  variable_get("basePathWimtv") . "userpacket/payment/pay?externalRedirect=true&success=" . urlencode($my_page));
            curl_setopt($ch, CURLOPT_VERBOSE, 0);

            curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
            curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
            curl_setopt($ch, CURLOPT_USERPWD, $credential);

            curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);

            curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
            curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
            curl_setopt($ch, CURLOPT_HTTPHEADER, array(
                    'Content-Type: application/json', 'Accept-Language: en-US,en;q=0.5',
                    'Content-Length: ' . strlen($data_string))
            );

            // salva cookie di sessione
            curl_setopt($ch, CURLOPT_COOKIEJAR, $fileCookie);
            $result = curl_exec($ch);
            curl_close($ch);
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

        $id_packet_user = $packet_user_json->id;
        $createDate_packet_user = $packet_user_json->createDate;
        $updateDate_packet_user = $packet_user_json->updateDate;

        $createDate = date('d/m/Y', $createDate_packet_user/1000);
        $updateDate = date('d/m/Y', $updateDate_packet_user/1000);
        $dateRange = getDateRange($createDate , $updateDate );

        $count_date = $packet_user_json->daysLeft;
        $response2 = apiCommercialPacket();

        $packet_json = json_decode($response2);
        $args = array('packet_json' => $packet_json,
            'id_packet_user' => $id_packet_user,
            'script' => $script);
        return render_template('templates/pricing.php', $args);
    }
}