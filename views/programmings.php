<?php
/**
 * Gestisce la sezione palinsesti del plugin
 */
function wimtvpro_programmings() {

    $view_page = wimtvpro_alert_reg();
    form_set_error("error", $view_page);

    if ($view_page!=""){
        return $view_page;
    }

    if (isset($_GET["functionList"]) && ($_GET["functionList"]=="delete")){
        $idProgrammingDelete = isset($_GET["id"]) ? $_GET["id"] : "";
        apiDeleteProgramming($idProgrammingDelete);
    }
    $response = json_decode(apiGetProgrammings());
    $programmings = $response->programmings;
    return render_template("templates/programmings/index.php", array("programmings"=>$programmings));
}

function wimtvpro_programming_edit() {

    $page = isset($_GET['namefunction']) ? $_GET['namefunction'] : "";
    if ($page=="new"){
        $nameProgramming = "";
        $progId = "";
    } else {
        $nameProgramming = "";
        $progId = isset($_GET["progId"]) ? $_GET["progId"] : "";
    }

    return render_template("templates/programmings/edit.php", array("nameProgramming" => $nameProgramming,
                                                                    "progId" => $progId));

}
