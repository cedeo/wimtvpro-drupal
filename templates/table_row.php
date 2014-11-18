<tr id="<?php echo $contentid ?>">
    <td class="image" style="width: 300px">
        <div class="wimtv-thumbnail">
            <span class="thumb-container">
                <?php echo($thumbnail); ?>
                <?php if ($license_type) { ?>
                    <div class="icon_licence <?php echo $license_type ?>"></div>
                <?php } ?>
            </span>
        </div>
        <b class="title"><?php echo html_entity_decode($title) ?></b>
    </td>

    <?php if($filename!==""):?>
        <td><?php echo t("Pending")."... (".$filename.")" ?></td>
        <td></td>
        <td></td>
        <td></td>
        </tr>
        
    <?php return; endif?>

    <td class="showtime">
        <span title="Remove from WimVod" class="icon_<?php echo $rmshowtime_class ?>" <?php echo $rmshowtime_style ?> id="<?php echo $publish_id ?>"></span>
        <span title="Add to WimVod" class="add icon_<?php echo $addshowtime_class ?>" <?php echo $addshowtime_style ?> id="<?php echo $publish_id ?>"></span>
        <div class="formVideo">
            <?php echo $form_video ?>
        </div>
    </td>

    <?php
    // NS: IF SHOWTIME (i.e. if we are in WimVod) SHOW shortcode
    $shortcode = "[wimtv]" . $publish_id . "|" . variable_get("widthPreview") . "|" . variable_get("heightPreview") . "[/wimtv]";
    if ($showtime) :
        ?>
        <td class="shortcode">
            <textarea class="icon_shortcode" style="resize: none; width:90%;height:100%;"readonly="true" title="Shortcode"><?php echo $shortcode ?></textarea>
        </td>
    <?php endif ?>
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