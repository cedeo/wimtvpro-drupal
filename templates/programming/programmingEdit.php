<?php
$title = isset($_GET["title"]) ? $_GET["title"] : "";
$progId = isset($_GET["progId"]) ? $_GET["progId"] : "";
print l("+" . t('Return to list'), 'admin/config/wimtvpro/programming');
?>


<div class='wrap'>
    <h2>
        <?php if (isset($_GET['progId'])) : ?>
            <div id="progform">
                <form>
                    <label><?php echo t("Give a name to this programming (not mandatory)"); ?></label>
                    <!--<input type="text" name='progname' value="<?php //echo $nameProgramming;   ?>" id="progname" />-->
                    <input type="text" name='progname' value="" id="progname" />
                    <input type="submit" value="<?php echo t("Send"); ?>" class="button button-primary submitnow" />
                    <input type="submit" value="<?php echo t("Skip"); ?>" class="button submitnow" />
                </form>
            </div>

            <!-- calendar -->
            <div id="calendar"></div>

            <div style="display:none">
                <div class="embedded">
                    <textarea id="progCode" onclick="this.focus();
                            this.select();"></textarea>
                </div>
            </div>   
        <?php endif ?>

