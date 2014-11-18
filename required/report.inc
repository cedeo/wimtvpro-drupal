<?php

/**
 * Created by JetBrains PhpStorm.
 * User: walter
 * Date: 17/12/13
 * Time: 15.37
 * To change this template use File | Settings | File Templates.
 */
function serializeStatistics($arrayStreams) {
    $streams = array();
    $megabyte = 1024 * 1024;
    foreach ($arrayStreams as $index => $stream) {
        $cid = (isset($stream->contentId) ? $stream->contentId : null);
        $details = json_decode(apiGetDetailsVideo($cid));

        $img = base_path() . drupal_get_path("module", "wimtvpro") . "/img/empty.jpg";
        $thumbs = "<img class='wimtv-thumbnail' src='" . $img . "' />";
        if (count($details)) {
            $img = $details->thumbnailUrl;

            $thumbs = "<img src='" . $img . "' />";
            $thumbs = str_replace('\"', '', $thumbs);
        }


        if ((isset($stream->title)))
            $stream->thumb = $thumbs . "<br/><b>" . $stream->title . "</b><br/>" . $stream->type;
        else
            $stream->thumb = $thumbs . "<br/>" . $stream->id;

        $stream->views_list = array();

        foreach ($stream->views_expanded as $value) {
            $value->traffic = round($value->traffic / $megabyte, 2) . " MB";
            $value->date_human = date('d/m/Y', ($value->end_time / 1000));
            array_push($stream->views_list, $value);
        }
        $streams[$index] = $stream;
    }
    return $streams;
}

function wimtvpro_report() {
    $view_page = wimtvpro_alert_reg();
    form_set_error("error", $view_page);
    if ($view_page == "") {

        drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/css/wimtvpro.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
        drupal_add_library('system', 'ui.datepicker');
        $script =
                'jQuery(document).ready(function(){
                  jQuery( ".pickadate" ).datepicker({ dateFormat: "dd/mm/yy",     maxDate: 0,      });
                  jQuery("#customReport").click(function(){
                      jQuery("#fr_custom_date").fadeToggle();
                      jQuery("#changeTitle").html("<a href=\'?page=WimVideoPro_Report\'>Current Month</a> Change Date");
                  });
                  jQuery(".tabs span").click(function(){
                      var idSpan = jQuery(this).attr("id");
                      jQuery(".view").fadeOut();
                      jQuery("#view_" + idSpan).fadeIn();
                      jQuery(".tabs span").attr("class","");
                      jQuery(this).attr("class","active");
                  });
              });';
        drupal_add_js($script, 'inline');

        $user = variable_get('userWimtv');
        $megabyte = 1024 * 1024;

        $from = isset($_POST['from']) ? $_POST['from'] : "";
        $to = isset($_POST['to']) ? $_POST['to'] : "";

        $dateNumber = array();
        $dateTraffic = array();

        if (($from != "") && ($to != "")) {
            list($day_from, $month_from, $year_from) = explode('/', $from);
            list($day_to, $month_to, $year_to) = explode('/', $to);

            $from_tm = mktime(0, 0, 0, $month_from, $day_from, $year_from) * 1000;
            $to_tm = mktime(0, 0, 0, $month_to, $day_to, $year_to) * 1000;


            $from_dmy = $month_from . "/" . $day_from . "/" . $year_from;
            $to_dmy = $month_to . "/" . $day_to . "/" . $year_to;

            $title_streams = "Streams (" . t("From") . " " . $from . " " . t("To") . " " . $to . ")";
            $title_user = "<a href='?page=WimTVPro_Report'>" . t("Current month") . "</a> " . t("Change Date");
            $style_date = "";
            $user_response = analyticsGetUser($from_tm, $to_tm);
            $traffic_json = json_decode($user_response);
            $traffic = $traffic_json->traffic;
            $storage = $traffic_json->storage;

            $packet = analyticsGetPacket();
            $commercialPacket_json = json_decode($packet);
            $currentPacket = $commercialPacket_json->current_packet;
            if (($currentPacket->id) > 0)
                $namePacket = $currentPacket->name;
            else
                $namePacket = $currentPacket->error;
            $byteToMb = "<b>" . round($traffic / $megabyte, 2) . ' MB</b>';
            $byteToMbS = "<b>" . round($storage / $megabyte, 2) . ' MB</b>';
        } else {
            $from_dmy = date("m") . "/01/" . date("y");

            $dayMe = cal_days_in_month(CAL_GREGORIAN, date("m"), date("y"));
            $to_dmy = date("m") . "/" . $dayMe . "/" . date("y");
            $from_tm = "";
            $to_tm = "";

            $title_streams = "Streams (" . t("Current month") . ")";
            $title_user = t("Current month") . " <a href='#' id='customReport'>" . t("Change Date") . "</a> ";
            $style_date = "display:none;";

            $user_response = analyticsGetUser();
            $traffic_json = json_decode($user_response);
            $traffic = $traffic_json->traffic;

            $packet = analyticsGetPacket();
            $commercialPacket_json = json_decode($packet);
            $currentPacket = $commercialPacket_json->current_packet;
            if (($currentPacket->id) > 0)
                $namePacket = $currentPacket->name;
            else
                $namePacket = $currentPacket->error;

            $traffic_of = " of " . $currentPacket->band_human;
            $storage_of = " of " . $currentPacket->storage_human;

            $traffic_bar = "<div class='progress'><div class='bar' style='width:" . $commercialPacket_json->traffic->percent . "%'>" . $commercialPacket_json->traffic->percent_human . "%</div></div>";
            $storage_bar = "<div class='progress'><div class='bar' style='width:" . $commercialPacket_json->storage->percent . "%'>" . $commercialPacket_json->storage->percent_human . "%</div></div>";

            $byteToMb = "<b>" . $commercialPacket_json->traffic->current_human . '</b>' . $traffic_of . $traffic_bar;
            $byteToMbS = "<b>" . $commercialPacket_json->storage->current_human . '</b>' . $storage_of . $storage_bar;
        }

        $response = analyticsGetStreams($from_tm, $to_tm);
        $arrayStreams = json_decode($response);

        $streams = serializeStatistics($arrayStreams);

        foreach ($streams as $stream) {
            foreach ($stream->views_expanded as $value) {
                if (isset($dateNumber[$value->date_human]))
                    $dateNumber[$value->date_human] = $dateNumber[$value->date_human] + 1;
                else
                    $dateNumber[$value->date_human] = 1;

                if (isset($dateTraffic[$value->date_human]))
                    array_push($dateTraffic[$value->date_human], $value->traffic);
                else
                    $dateTraffic[$value->date_human] = array($value->traffic);
            }
        };

        $args = array('user' => $user,
            'streams' => $streams,
            'traffic' => $traffic,
            'from' => $from,
            'to' => $to,
            'dateNumber' => $dateNumber,
            'dateTraffic' => $dateTraffic,
            'from_dmy' => $from_dmy,
            'to_dmy' => $to_dmy,
            'title_streams' => $title_streams,
            'title_user' => $title_user,
            'style_date' => $style_date,
            'namePacket' => $namePacket,
            'byteToMb' => $byteToMb,
            'byteToMbS' => $byteToMbS);
        return render_template('templates/report.php', $args);
    }
    return $view_page;
}
