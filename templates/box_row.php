<li>
    <?php if ($is_field) { ?>
        <a class='addThumb' href='#' id='<?php echo $publish_id ?>'><?php echo t("Add") ?></a>
        <a class='removeThumb' href='#' id='<?php echo $publish_id ?>'><?php echo t("Remove") ?></a>
    <?php } ?>
    <a class="viewThumbsPublic" title="<?php echo $title;?>" href="#" id="<?php echo $preview ?>">
    <?php echo $thumbnail ?>
    <b class="title"><?php echo $title ?></b>
    </a>
    <?php if ($is_field) { ?>
        <p>W <input style='width:20px;' maxweight='3' class='w' type='text' value='<?php echo variable_get("widthPreview") ?>'>px  -  H <input style='width:20px;' maxweight='3' class='h' type='text' value='<?php echo variable_get("heightPreview") ?>'>px</p>
    <?php } ?>
</li>
