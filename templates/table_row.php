<tr>
    <td class="image">
        <span class="wimtv-thumbnail">
            <?php echo $thumbnail ?>
            <?php if ($license_type) { ?>
                <div class="icon_licence <?php echo $license_type ?>"></div>
            <?php } ?>
        </span>
        <br />
        <b class="title"><?php echo $title ?></b>
    </td>
    <td class="showtime">
        <span title="Remove from WimVod" class="<?php echo $rmshowtime_class ?>" <?php echo $rmshowtime_style ?> id="<?php echo $publish_id ?>"></span>
        <span title="Add to WimVod" class="<?php echo $addshowtime_class ?>" <?php echo $addshowtime_style ?> id="<?php echo $publish_id ?>"></span>
        <div class="formVideo">
            <?php echo $form_video ?>
        </div>
    </td>
    <td class="download">
        <span class="icon_download" id="<?php echo $contentid ?>" title="Download"></span>
    </td>
    <td class="view">
        <a class="viewThumb" title="Preview Video" href="#" id="<?php echo $preview ?>">
            <span class="icon_view"></span>
        </a>
    </td>
    <td class="delete">
        <?php if ($remove) { ?>
            <span title="Remove" class="icon_remove" id="<?php echo $remove ?>"></span>
        <?php } ?>
    </td>
</tr>
