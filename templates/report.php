<?php

function writeGraph($from_dmy, $to_dmy, $dateNumber, $dateTraffic) {
    $dateRange = getDateRange($from_dmy, $to_dmy);
    $count_date = count($dateRange);
    $count_single= 0;
    $traffic_single = 0;
    if (count($dateNumber)>0) {
        $number_view_max = max($dateNumber);
        $single_percent = (100/$number_view_max);
    }
    else
        $single_percent = 0;
    $single_traffic_media = array();
    foreach ($dateTraffic as $dateFormat => $traffic_number){
        $single_traffic_media[$dateFormat] = round(array_sum($dateTraffic[$dateFormat]) / count($dateTraffic[$dateFormat]),2);
    }
    if (count($single_traffic_media)>0) {
        $traffic_view_max = max($single_traffic_media);
        if ($traffic_view_max!=0)
            $single_traffic_percent = (100/$traffic_view_max);
        else
            $single_traffic_percent = 0;
    }
    else {
        $traffic_view_max = 0;
    }


    echo "<div id='view_graph' class='view'>";

    echo "<div class='cols'>";


    echo "<div class='col'><div class='date'>" . t("Date") . "</div><div class='title'>" . t("Total viewers") . "</div><div class='title'>" . t("Average Traffic") . "</div></div>";
    for ($i=0;$i<$count_date;$i++){
        if (isset($dateNumber[$dateRange[$i]]))
            $count_single = $single_percent * $dateNumber[$dateRange[$i]];
        if (isset($single_traffic_media[$dateRange[$i]]))
            $traffic_single = $single_traffic_percent * $single_traffic_media[$dateRange[$i]];

        echo "<div class='col' >
                <div class='date'>" . $dateRange[$i] . "</div>
                <div class='countview'><div class='bar' style='width:" . $count_single . "%'>";
        if (isset($dateNumber[$dateRange[$i]])) {
            if ($dateNumber[$dateRange[$i]]>1)
                echo $dateNumber[$dateRange[$i]] . " " . t("viewers");
            else if ($dateNumber[$dateRange[$i]]==1)
                echo $dateNumber[$dateRange[$i]] . "  " . t("viewer");
        }
        echo "</div></div>
                <div class='countview'><div class='barTraffic' style='width:" . $traffic_single . "%'>";
        if (isset($single_traffic_media[$dateRange[$i]]) && $single_traffic_media[$dateRange[$i]]>0)
            echo $single_traffic_media[$dateRange[$i]] . " MB";
        echo "</div></div>
                </div>";
        $count_single = 0;
        $traffic_single = 0;
    }

    echo "</div>";
    echo "<div class='clear'></div>
          </div>
          <div class='clear'></div>
          </div>";
}

?>

<div class='wrap'>
<h2>Report user Wimtv <?php echo $user ?></h2>
<h3 id='changeTitle'><?php echo $title_user ?></h3>

<div class="registration" id="fr_custom_date" style="<?php echo $style_date ?>">
    <form method="post">
        <fieldset>
            <span><?php echo  t("From") ?></span>
            <input  type="text" class="pickadate" id="edit-from" name="from" value="<?php echo $from ?>" size="10" maxlength="10" />
            <span><?php echo  t("To") ?></span>
            <input  type="text" class="pickadate" id="edit-to" name="to" value="<?php echo $to ?>" size="10" maxlength="10" />
            <input type="submit" value=">" class="button button-primary" />
        </fieldset>
    </form>
</div>
<p><?php echo t("You commercial packet") ?>:
    <b><?php echo $namePacket ?></b> - <a href='../../wimtvpro-drupal/templates/?page=WimTvPro&pack=1&return=WimTVPro_Report'><?php echo t("Change") ?></a>
</p>
<?php if ($traffic == "") { ?>
    <p><?php echo t("You did not generate any traffic in this period") ?></p>
<?php } else { ?>
<p><?php echo t("Traffic") . ": " . $byteToMb ?></p>
<p><?php echo t("Storage space") . ": " . $byteToMbS ?></p>
<div class="summary">
    <div class="tabs">
        <span id="stream" class="active"><?php echo  t("View Streams") ?></span>
        <span id="graph"><?php echo t("View graphic") ?></span>
    </div>
    <div id="view_stream" class="view">
        <h3><?php echo $title_streams ?></h3>
        <table class="wp-list-table widefat fixed posts" style="text-align:center;">
            <tr>
                <th class="manage-column column-title">Video</th>
                <th class="manage-column column-title"><?php echo  t("Viewers") ?></th>
                <th class="manage-column column-title"><?php echo  t("Active Viewers") ?></th>
                <th class="manage-column column-title"><?php echo  t("Max viewers") ?></th>
            </tr>
            <?php foreach($streams as $stream) { ?>
                <tr class='alternate'>
                    <td class='image'><?php echo $stream->thumb ?></td>
                    <td>
                        <b><?php echo  t("Total") . ": " . $stream->views . " " . t("Viewers") ?></b>
                        <div class="wp-list-table">
                            <table class='wp-list-table'>
                                <tr>
                                    <th class='manage-column column-title' style='font-size:10px;'><?php echo t("Date") ?></th>
                                    <th class='manage-column column-title' style='font-size:10px;'><?php echo t("Duration") ?></th>
                                    <th class='manage-column column-title' style='font-size:10px;'><?php echo t("Traffic") ?></th>
                                </tr>
                                <?php foreach($stream->views_list as $value) { ?>
                                    <tr>
                                        <td style='font-size:10px;'><?php echo $value->date_human ?></td>
                                        <td style='font-size:10px;'><?php echo $value->duration ?>s</td>
                                        <td style='font-size:10px;'><?php echo $value->traffic  ?></td>
                                    </tr>
                                <?php } ?>
                            </table>
                        </div>
                    </td>
                    <td><?php echo $stream->viewers ?></td>
                    <td><?php echo $stream->max_viewers ?></td>
                </tr>

            <?php
            }
            ?>
        </table>
        <div class='clear'>
        </div>
    </div>


<?php
    writeGraph($from_dmy, $to_dmy, $dateNumber, $dateTraffic);
}
?>
</div>