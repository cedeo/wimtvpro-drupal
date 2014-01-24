<div class="help"><a href="http://support.wim.tv/?cat=5" target="_new">Help</a></div>
<?php echo l(t("Add") . " " . t("new event"), "admin/config/wimtvpro/wimlive/insert") ?>
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