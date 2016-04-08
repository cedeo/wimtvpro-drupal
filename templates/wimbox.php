<?php render_help_link();?>

<div class='action'>
    <span class='icon_sync0' title='Syncronize'><a href="javascript:void(0)" class='button'><?php echo t("Synchronize"); ?></a></span>
</div>

<div class="region region-help">
    <div id="block-system-help" class="block block-system">


        <div class="content">
            <p>
                <?php echo t("This page gives you the list of all videos you have uploaded to your personal WimTV repository. If you wish to post one of these videos to your site, move it to WimVod by clicking the corresponding icon"); ?>

            </p>
        </div>
    </div>
</div>

<?php
print l("+" . t('Upload video'), 'admin/config/' . getWhiteLabel('APP_NAME') . '/' . getWhiteLabel('UPLOAD_urlLink'));

// NS: RENDER "VIDEOS SEARCH FORM" AND "VIDEOS FOUND FORM"
//if ($thumbs) {
$form_wimtvpro_videosearch = drupal_get_form('wimtvpro_videosearch_form');
echo drupal_render($form_wimtvpro_videosearch);
$form_wimtvpro_videos_found = drupal_get_form('wimtvpro_videos_found_form', array('total_video_count' => $total_video_count));
echo drupal_render($form_wimtvpro_videos_found);
//}
?>

<table class='items' id='FALSE'>
    <thead>
        <tr>
            <th>Video</th>
            <th><?php echo t("Status"); ?></th>
            <th>Download</th>
            <th><?php echo t("View"); ?></th>
            <th><?php echo t("Delete"); ?></th>
        </tr>
    </thead>
    <tbody>
        <?php echo $thumbs ?>
    </tbody>
</table>