<?php echo $script ?>
<table class='wp-list-table widefat fixed pages'>
    <thead>
    <tr>
    <th></th>
    <?php foreach ($packet_json -> items as $a) { ?>
        <th><b><?php echo $a->name ?></b></th>
    <?php } ?>
    </thead>
    <tbody>
    <tr class='alternate'>
        <td><?php echo t("Bandwidth") ?></td>
        <?php foreach ($packet_json -> items as $a) { ?>
            <td><?php echo $a->band ?> GB</td>
        <?php } ?>
    </tr>
    <tr>
        <td><?php echo t("Storage") ?></td>
        <?php foreach ($packet_json -> items as $a) { ?>
            <td><?php echo $a->storage ?> GB</td>
        <?php } ?>
    </tr>
    <tr class='alternate'>
        <td><?php echo t("Support") ?></td>
        <?php foreach ($packet_json -> items as $a) { ?>
            <td><?php echo $a->support ?></td>
        <?php } ?>
    </tr>
    
    <tr>
        <td><?php echo t("Hours of transmission(*)") ?></td>
        <?php foreach ($packet_json -> items as $a) { ?>
            <td><?php echo $a->streamingAmount;?></td>
        <?php } ?>
    </tr>
    
    <tr>
        <td><?php echo t("Price/mo. for 1 Mo (**)") ?></td>
        <?php foreach ($packet_json -> items as $a) { ?>
            <td><?php echo number_format($a->price,2) ?> &euro; / m</td>
        <?php } ?>
    </tr>
    <tr class='alternate'>
        <td></td>
        <?php foreach ($packet_json -> items as $a) { ?>
            <td><?php echo $a->dayDuration . " - " . $a->id ?><br/>
                <?php if ($id_packet_user==$a->id) { ?>
                    <img  src='<?php echo base_path()  . drupal_get_path('module', 'wimtvpro') ?>/img/check.png' title='Checked'><br/>
                    <?php if ($a->id>1) echo $count_date . " " . t("day left") ?>
                <?php } else { ?>
                    <a href='<?php echo base_path() . '?q=admin/config/wimtvpro&pack=1&upgrade=' . $a->name ?>'>
                        <img class='icon_upgrade' src='<?php echo base_path() . drupal_get_path('module', 'wimtvpro') ?>/img/uncheck.png' title='Upgrade'><br/>
                    </a>
                <?php } ?>
            </td>
        <?php } ?>
    </tr>
    </tbody>
</table>


<p><strong> <?php echo t("(*) Assuming that video+audio payload is 1 Mbps"); ?> </strong>
<br/><strong> <?php echo t("(**) VAT to be added"); ?> </strong></p>

<p><?php echo t("If before the end of the month you consume"); ?></p>

<p><?php echo t("80% of your package (either storage or bandwidth) you will be notified by email"); ?></p>
<p><?php echo t("100% of your package (either storage or bandwidth) you will be asked to upgrade to another package."); ?></p>
<p><?php echo t("Note that, if you stay within the usage limits of the Free Package, use of WimTV is free"); ?></p>
<p><?php echo t("If you license content and/or provide services in WimTV, revenue sharing will apply"); ?></p>

<p><strong><?php echo t("Enjoy your WimTVPro plugin!"); ?></strong></p>

