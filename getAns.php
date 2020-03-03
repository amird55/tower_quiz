<?php
include "functions.php";
$dev_id=(isset($_GET['dev']))?$_GET['dev']:1;
$mysqli=connect_to_db();
$params=getGameParams($mysqli,$dev_id);
if($params['game_status'] == "G"){
    $ans=(isset($_GET['a']))?$_GET['a']:-1;
    $ret=1;
    if($ans==$params['curr_ans']){
        $ret=2;
        $p=getNextQuestion($mysqli,$dev_id,$params['prev_q']);
//        var_dump($p);
    }
    if($params['q_num']>=10){
        $ret=4;
    }
}
echo "$ret\n";