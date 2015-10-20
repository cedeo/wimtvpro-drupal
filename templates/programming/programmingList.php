<div class="region region-help">
    <div id="block-system-help" class="block block-system">


        <div class="content">
            <p>
                <?php echo t("This page gives you the list of all Programmings."); ?>

            </p>
        </div>
    </div>
</div>
<?php print l("+" . t('New'), 'admin/config/wimtvpro/programming/add'); ?>

<table id='tableProg' class='items'>
    <thead>
        <tr>
            <th><?php echo t("Title"); ?></th>
            <th><?php echo t("Modify"); ?></th>
            <th><?php echo t("Remove"); ?></th>
            <!--<th><?php echo t("Shortcode"); ?></th>-->
        </tr>
    </thead>
    <tbody>

        <?php
        foreach ($arrayjsonst->programmings as $prog) :
            if (!isset($prog->name))
                $titleProgramming = t("No title");
            else
                $titleProgramming = $prog->name;
            ?>
            <tr>
                <td><?php echo $titleProgramming; ?></td>
                <?php
                $path_edit = "/admin/config/wimtvpro/programming/edit?title=" . $titleProgramming . "&progId=" . $prog->identifier;
                $path_delete = "/admin/config/wimtvpro/programming/delete?progId=" . $prog->identifier;
                ?>
                <td>
                    <a href='<?php echo $path_edit ?>' alt='<?php t("Modify"); ?>' title='<?php t("Modify"); ?>'>
                        <img src='<?php echo base_path() . drupal_get_path("module", "wimtvpro") . "/img/mod.png"; ?>'  alt='<?php t("Modify"); ?>'>
                    </a>
                </td>
                <td>
                    <!--<a href='?page=WimVideoPro_Programming&functionList=delete&id=<?php echo $prog->identifier; ?>' alt='<?php t("Remove"); ?>' title='<?php t("Remove"); ?>'>-->
                    <a href='<?php echo $path_delete ?>' alt='<?php t("Modify"); ?>' title='<?php t("Modify"); ?>'>
                        <img src='<?php echo base_path() . drupal_get_path("module", "wimtvpro") . "/img/remove.png"; ?>'  alt='<?php t("Remove"); ?>'>
                    </a>
                </td>
<!--                <td>

                    <textarea style="resize: none; width:90%;height:100%;" readonly='readonly' 
                              onclick="this.focus();
                                      this.select();">[wimprog id="<?php echo $prog->identifier; ?>"]</textarea>

                </td>-->
            </tr>
        <?php endforeach; ?>
    </tbody>
</table>


