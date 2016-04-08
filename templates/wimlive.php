<?php render_help_link(); ?>

<div class="region region-help">
    <div id="block-system-help" class="block block-system">


        <div class="content">
            <p> <?php echo t("This page lets you create and post live streaming events to your website."); ?></p>

            <p> <?php echo t("You need an audio video encoder (producer) to stream an event captured by a camera. You have two choices:"); ?></p>

            <ol>
                <li><?php echo t("If you have an external video camera connected to your PC, install a video encoding software (e.g. Adobe Flash Media Live Encoder, Wirecast etc.)."); ?></li>
                <li><?php echo t('If you broadcast directly from your web cam, click "Live now" icon to open the WimTV producer will open in a new browser tab. Remeber to keep it open during the whole transmission.'); ?></li>
            </ol>

        </div>
    </div>
</div>

<?php echo l(t("Add") . " " . t("new event"), 'admin/config/' . getWhiteLabel('APP_NAME') . '/' . getWhiteLabel('WIMLIVE_urlLink') . "/insert") ?>
<table id='tableLive'>
    <thead>
        <tr>
            <th>Name</th>
            <th>Live Now</th>
            <th>Pay-Per-View</th>
            <th>URL</th>
            <th>Streaming</th>
            <th>Embed Code</th>
            <th></th>
        </tr>
    </thead>
    <tbody>
        <?php echo $elenco ?>
    </tbody>
</table>