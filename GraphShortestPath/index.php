<?php


//从某数组中删掉元素
function unset_key_in_array(&$array, $aim)
{
    foreach( $array as $key=>$value )
    {
        if( $value==$aim )
        {
            unset($array[$key]);
            break;
        }
    }
}

//返回最小值的节点
function array_min( &$array )
{
    $min_value=null;
    foreach($array as $key=>$value)
    {
        if( $min_value===null )
        {
            $node=$key;
            $min_value=$value;
        }

        if( $value<$min_value )
        {
            $min_value=$value;
            $node=$key;
        }
    }
    return $node;
}

//获取邻居节点
function get_neighbor_nodes($neighbors_info)
{
    $nodes=array();
    foreach( $neighbors_info as $neighbor )
    {
        $nodes[]=$neighbor['name'];
    }
    return $nodes;
}

function neighbor_intersect(&$neighbor, &$U)
{
    $result=array();
    foreach( $neighbor as $item1 )
    {
        foreach($U as $item2)
        {
            if( $item2==$item1['name'] )
            {
                $result[]=$item1;
            }
        }
    }
    return $result;
}


/*
*    加载北京地铁信息
 */
function  load_subway_info()
{
    global $lines_info;
    $file=fopen('map_bjsubway_leon.txt', 'rt');
    if($file)
    {
        $cur_line='';
        while (($buffer=fgets($file, 4096)) !== false)
        {
            if( strstr($buffer, '[Line=')!==FALSE )
            {
                preg_match('/\[Line=(?P<line>\S+)\s*(?P<loop>[Loop]*)\]/', $buffer, $match);
                if( strcmp($match['loop'],'Loop' )==0)
                {
                    $is_loop=true;
                }
                else
                {
                    $is_loop=false;
                }
                $cur_line=$match['line'];
                $lines_info[$cur_line]=array('sites'=>array(), 'loop'=>$is_loop);
            }
            else if (strstr($buffer, '[End]')!==FALSE)
            {
                $cur_line='';
            }
            else if( strstr($buffer, '=')!==FALSE )
            {
                preg_match('/(?P<name>\S+)\s*=\s*(?P<pinyin>\w+)\s*<(?P<distance>[0-9\.]+)>/' ,$buffer, $match);
                if( count($match)==7 )
                {
                    $site_name=$match['name'];
                    $site_pinyin=$match['pinyin'];
                    $distance=$match['distance'];
                    $lines_info[$cur_line]['sites'][]=array('name'=>$site_name, 'pinyin'=>$site_pinyin, 'distance'=>$distance );
                }
                else
                {
                    echo $buffer . '<br>';
                }
            }
        }
        fclose($file);
        return true;
    }
    else
    {
        echo "文件打开失败";
        return false;
    }
}

/*
 *    解析北京地铁信息
 * */
function parse_subway_info( )
{
    global $site_info, $lines_info;
    foreach( $lines_info as $line_code=>$line_info )
    {
        $is_loop=$line_info['loop'];  //本线路属性
        $sites=&$line_info['sites'];  //本线路所有站点
        $count=count($sites);         //本线路站点个数

        $first_site=$sites[0];
        $last_site=$sites[$count-1];
        for($i=0; $i<$count; $i++)
        {
            $cur_site=$sites[$i];

            //从节点映射表中取出节点
            if( !isset($site_info[$cur_site['name']]))
            {
                $site_info[$cur_site['name']]=array('name'=>$cur_site['name'], 'neighbor'=>array(), 'line'=>array());
            }
            $node=&$site_info[$cur_site['name']];

            if( $i==0 )
            {  //如果是第一个站点
                if( $is_loop )
                {
                    $node['neighbor'][]=array('name'=>$last_site['name'], 'distance'=>floatval($last_site['distance']));
                }
                $node['neighbor'][]=array('name'=>$sites[$i+1]['name'], 'distance'=>floatval($cur_site['distance']));
            }
            else if( $i==$count-1 )
            {   //如果是最后一个站点
                if( $is_loop )
                {
                    $node['neighbor'][]=array('name'=>$first_site['name'], 'distance'=>floatval($cur_site['distance']));
                }
                $node['neighbor'][]=array('name'=>$sites[$i-1]['name'], 'distance'=>floatval($sites[$i-1]['distance']));
            }
            else
            {   //中间的节点
                $node['neighbor'][]=array('name'=>$sites[$i-1]['name'] , 'distance'=>floatval($sites[$i-1]['distance']));
                $node['neighbor'][]=array('name'=>$sites[$i+1]['name'] , 'distance'=>floatval($sites[$i]['distance']));
            }

            //本节点归属于哪些路线
            $node['line'][]=$line_code;
        }
    }
}


/*
*  迪杰斯特拉最短路径算法
 */
function  shortest_path($start_site)
{
    global $site_info;

    $S=array();  //array('aa'=>1, 'bb'=>1.1, 'cc'=>3.2)
    $U=array();

    $Un=array_keys($site_info);  //array(''aa,''bb,''cc)
    unset_key_in_array($Un, $start_site); //除开始节点的其他节点

    $last_distance=0.0;
    $select_node=$start_site;

    while(count($Un)>0)
    {
        //遍历
        $nodes=neighbor_intersect($site_info[$select_node]['neighbor'], $Un);
        foreach( $nodes as $item )
        {
            $new_distance=$last_distance+$item['distance'];
            if( isset($U[$item['name']]) )
            {
                if( $U[$item['name']]>$new_distance )
                {
                    $U[$item['name']]=$new_distance;
                }
            }
            else
            {
                $U[$item['name']]=$new_distance;
            }
        }

        //U集合中取最小值
        $select_node=array_min($U);

        //从U集合中删掉节点加入到S集合中
        $S[$select_node]=$U[$select_node];
        $last_distance=$U[$select_node];
        unset($U[$select_node]);
        unset_key_in_array($Un, $select_node);
    }

    return $S;
    //return  $S[$end_site];
}

$site_info=array();
$lines_info=array();

$t1 = microtime(true);
load_subway_info();
parse_subway_info();
$t2 = microtime(true);

$result=shortest_path('西二旗');
$t3=microtime(true);

$parse_cost_time=round($t2-$t1,3);
$path_cost_time=round($t3-$t2,3);

?>


<html>
<head>
    <meta charset="UTF-8">
</head>

<body>
    <h1>起始地点: 西二旗</h1>
    <h1>解析耗时:<?php echo $parse_cost_time;?>秒 .计算耗时: <?php echo $path_cost_time;?>秒</h1>
    <table>
        <thead>
            <tr>
                <th>目的地</th>
                <th>距离(公里)</th>
            </tr>
        </thead>
        <tbody>
            <?php foreach($result as $key=>$value){ ?>
            <tr>
                <td><?php echo $key;?></td>
                <td><?php echo $value;?>km</td>
            </tr>
            <?php } ?>
        </tbody>
    </table>
</body>
</html>
