<?php
$title = isset($_GET["title"]) ? $_GET["title"] : "";
$progId = isset($_GET["progId"]) ? $_GET["progId"] : "";
//print l("+" . t('Return to list'), 'admin/config/wimtvpro/programming');
print l("+" . t('Return to list'), 'admin/config/' . getWhiteLabel('APP_NAME') . '/' . getWhiteLabel('SCHEDULES_urlLink') );

echo apiProgrammingGetIframe($progId);
?>