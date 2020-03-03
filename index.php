<?php
include "functions.php";
/* get data from db
 * 
 * game phase (wait / going / finished)
 * questions asked
 * current question number
 * current question status (asked / answered)
 */

$dev_id=(isset($_GET['dev']))?$_GET['dev']:1;
$mysqli=connect_to_db();
$params=getGameParams($mysqli,$dev_id);
if(count($params)==0){
    //new game
    $params=beginNewGame($mysqli,$dev_id);
}
if(isset($_GET['W'])){
    $params=getReadyForNewGame($mysqli,$dev_id);
}
if(isset($_GET['N'])){
    $params=startNewGame($mysqli,$dev_id);
}
if(isset($_GET['F'])){
    $params=finishGame($mysqli,$dev_id);
}
//var_dump($params);
$content='';
switch ($params['game_status']) {
    case 'W':
        ob_start();
        include "tpl_wait_2_start.php";
        $content = ob_get_contents(); 
        ob_end_clean();
        break;
    case 'F':
        ob_start();
        include "tpl_finish.php";
        $content = ob_get_contents(); 
        ob_end_clean();
        break;
    case 'G':
        $qData=getCurrentQuestion($mysqli,$dev_id,$params['curr_q_id']);
        $q=$qData['q'];
        $a=array();
        $a[$params['curr_ans']]=$qData['correct'];
        $a[$params['ans1_pos']]=$qData['ans1'];
        $a[$params['ans2_pos']]=$qData['ans2'];
        $a[$params['ans3_pos']]=$qData['ans3'];
        ob_start();
        include "tpl_question.php";
        $content = ob_get_contents(); 
        ob_end_clean();
        break;

    default:
        break;
}


        include_once "tpl_basic.php";

        
        