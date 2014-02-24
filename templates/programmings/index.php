<div class="help"><a href="http://support.wim.tv/category/wimtvpro-drupal/" target="_new">Help</a></div>

<a style="top: 10px; position: relative" href='<?php echo url('admin/config/wimtvpro/programmings/edit', array("query" => array("namefunction" => "new"))); ?>' class='button form-submit'>+ <?php echo t('New') ?></a>

<table style="margin-top: 30px">
    <thead>
        <tr>
            <th>Title</th>
            <th>Edit</th>
            <th>Remove</th>
            <!--th><?php t("Shortcode");?></th-->
        </tr>
    </thead>
    <tbody>
    <?php foreach ($programmings as $prog){
        if (!isset($prog->name) )
            $titleProgramming = t("No title");
        else
            $titleProgramming = $prog->name;
        ?>
        <tr>
            <td><?php echo $titleProgramming; ?></td>
            <td>
                <a href="<?php echo url('admin/config/wimtvpro/programmings/edit', array("query" => array("progId" => $prog->identifier, "title" => $titleProgramming))); ?>">
                    <?php echo t("Edit") ?>
                </a>
            </td>
            <td>
                <a href="<?php echo url('admin/config/wimtvpro/programmings', array("query" => array("functionList" => "delete", "id" => $prog->identifier))); ?>">
                    <span title="Remove" class="icon_remove"></span>
                    <?php echo t("Remove") ?>
                </a>
            </td>
            <!--td>
                <textarea style="resize: none; width:90%;height:100%;" readonly='readonly'
                          onclick="this.focus(); this.select();">[wimprog id="<?php echo $prog->identifier;?>"]</textarea>
            </td-->
        </tr>
    <?php
    }
    ?>
    </tbody>
</table>

<?php drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/css/wimtvpro.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE)); ?>