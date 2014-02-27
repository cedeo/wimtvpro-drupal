<a href='<?php echo url('admin/config/wimtvpro/programmings'); ?>' class='add-new-h2'><?php echo t('Return to list') ?></a>
<br />
<style type="text/css">@import url("<?php echo substr(variable_get('basePathWimtv'), 0, -6) ?>/css/fullcalendar.css")</style>
<style type="text/css">@import url("<?php echo substr(variable_get('basePathWimtv'), 0, -6) ?>/css/programming.css")</style>
<style type="text/css">@import url("<?php echo substr(variable_get('basePathWimtv'), 0, -6) ?>/css/jquery-ui/jquery-ui.custom.min.css")</style>
<style type="text/css">@import url("<?php echo substr(variable_get('basePathWimtv'), 0, -6) ?>/css/jquery.fancybox.css")</style>
<div id="progform">
    <form>
        <label><?php t("Give a name to this programming (not mandatory)"); ?></label>
        <input type="text" value="<?php echo $nameProgramming;?>" id="progname" class="form-text"/>
        <input type="submit" value="Send" class="button submitnow form-submit" />
        <input type="submit" value="Skip" class="button submitnow form-submit" />
    </form>
</div>
<!-- calendar -->
<div id="calendar"></div>

<div style="display:none">
    <div class="embedded">
        <textarea id="progCode" onclick="this.focus(); this.select();"></textarea>
    </div>
</div>
<?php
    global $base_url;
    $baseRoot = substr(variable_get('basePathWimtv'), 0, -6);
    drupal_add_js("var url_pathPlugin ='" . $base_url . "/';" , "inline");
    $base = url("admin/config/wimtvpro/programming-api");
    drupal_add_js("var programmingBase ='" . $base . "';" , "inline");
    drupal_add_js("var imageBase ='" . $baseRoot . "';" , "inline");
    drupal_add_js('https://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js');
    drupal_add_js($baseRoot . '/script/jquery-ui.custom.min.js');
    drupal_add_js($baseRoot . '/script/jquery.fancybox.min.js');
    drupal_add_js($baseRoot . '/script/jquery.mousewheel.min.js');
    drupal_add_js($baseRoot . '/script/fullcalendar/fullcalendar.min.js');
    drupal_add_js($baseRoot . '/script/utils.js');
    drupal_add_js($baseRoot . '/script/programming/programming.js');
    drupal_add_js($baseRoot . '/script/programming/calendar.js');
    drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/programming-api.js');
    drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/css/wimtvpro.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
?>