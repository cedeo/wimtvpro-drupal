<li id="<?php echo $contentid ?>">
    <div class="image" style="width: 300px">
        <div class="wimtv-thumbnail">
            <span class="thumb-container">
                <?php echo $thumbnail ?>
                <?php if ($license_type) { ?>
                    <div class="icon_licence <?php echo $license_type ?>"></div>
                <?php } ?>
            </span>
        </div>
        <b class="title"><?php echo $title ?></b>
    </div>
    <div class="showtime">
        <span title="<?php echo t("Remove from WimVod");?>" class="icon_<?php echo $rmshowtime_class ?>" <?php echo $rmshowtime_style ?> id="<?php echo $publish_id ?>"></span>
        <span title="Add to WimVod" class="add icon_<?php echo $addshowtime_class ?>" <?php echo $addshowtime_style ?> id="<?php echo $publish_id ?>"></span>
        <div class="formVideo">
            <?php echo $form_video ?>
        </div>
    </div>
    <div class="download">
        <span class="icon_download" id="<?php echo $contentid ?>" title="Download"></span>
    </div>
    <div class="view">
        <a class="viewThumb" title="<?php echo t("Preview Video");?>" href="#" id="<?php echo $preview ?>">
            <span class="icon_view"></span>
        </a>
    </div>
    <div class="delete">
        <?php if ($remove) { ?>
            <span title="<?php echo t("Remove");?>" class="icon_remove" id="<?php echo $remove ?>"></span>
        <?php } ?>
    </div>
</li>
