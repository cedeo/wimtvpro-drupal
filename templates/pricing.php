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
        <td><?php echo t("Band") ?></td>
        <?php foreach ($packet_json -> items as $a) { ?>
            <td><?php echo $a->band ?> GB</td>
        <?php } ?>
    </tr>
    <tr>
        <td><?php echo t("Storage") ?></td>
        <?php foreach ($packet_json -> items as $a) { ?>
            <td><?php $a->storage ?> GB</td>
        <?php } ?>
    </tr>
    <tr class='alternate'>
        <td><?php echo t("Support") ?></td>
        <?php foreach ($packet_json -> items as $a) { ?>
            <td><?php echo $a->support ?></td>
        <?php } ?>
    </tr>
    <tr>
        <td><?php echo t("Price") ?></td>
        <?php foreach ($packet_json -> items as $a) { ?>
            <td><?php echo number_format($a->price,2) ?> &euro; / month</td>
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

<p>You have a free trial of 30 days to try the WimTVPro plugin.<br/>
    After 30 days you can subscribe a plan that suit your needs.<br/>
    All plans come with all features, only changes the amount of bandwidth and storage available.<br/>
    Enyoy your WimTVPro video plugin!</p>