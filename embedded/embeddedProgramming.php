<?php

function wimtvpro_programming_embedded($progId) {
    $basePath = cms_getWimtvApiUrl();
    $height = variable_get("heightPreview") + 100;
    $width = variable_get("widthPreview");
    $iframe = '<div class="wrapperiframe" style="max-width:"' . $width . 'px" ><div class="h_iframe"><iframe src="' . $basePath . 'programming/' . $progId . '/embedded" frameborder="0" allowfullscreen style="overflow:hidden;" style="height:"' . $height . 'px;width:2px"></iframe></div></div>';
    return $iframe;
}

?>