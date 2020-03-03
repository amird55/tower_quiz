<?php
include "functions.php";
$dev_id=(isset($_GET['dev']))?$_GET['dev']:1;
$mysqli=connect_to_db();
if(isset($_GET['W'])){
    $params=getReadyForNewGame($mysqli,$dev_id);
    echo "W \n";
}
if(isset($_GET['N'])){
    $params=startNewGame($mysqli,$dev_id);
    echo "N \n";
}
if(isset($_GET['F'])){
    $params=finishGame($mysqli,$dev_id);
    echo "F \n";
}
//var_dump($params);
        