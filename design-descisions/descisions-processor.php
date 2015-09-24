<?php
/**
 * @Author: Nate Bosscher
 * @Date:   2015-09-24 18:53:23
 * @Last Modified by:   Nate Bosscher
 * @Last Modified time: 2015-09-24 18:54:04
*/

$processor = new Descision("Processor"); // all $p_AAAA are items which pertain to the processor
{
    $p_cost = new Descision("Processor Size");
    $p_cost->addAffects("Cost");

    $p_size = new Descision("Processor Size");
    $p_size->addAffects("Processor Enclosure");

    $p_io = new Descision("IO Ports"); // affected by user experience/calibration method
    {
        $p_sensor_config = new Descision("Sensor Configuration"); // affected by sensor requirements
        $p_sensor_config->addReliesOn("Sensor Requirements");

        $p_io
            ->addSubDescision($p_sensor_config)
            ->addReliesOn("User Experience")
            ->addReliesOn("Calibration Method");
    }

    $p_speed = new Descision("Processor Speed");
    {
        $p_command_t = new Descision("Command time/Freq"); // affected by sensor configuration, sensor control
        $p_command_t
            ->addReliesOn("Sensor Configuration")
            ->addReliesOn("Sensor Control");

        $p_data_method = new Descision("Data Processing Method"); // affected by user experience
        $p_data_method
            ->addReliesOn("User Experience");

        $p_speed
              ->addSubDescision($p_command_t)
              ->addSubDescision($p_data_method);
    }

    $p_network = new Descision("Network Capabilities");
    $p_network
        ->addReliesOn("User Experience")
        ->addReliesOn("Data Size")
        ->addOption(function(){ 
            $opt = new Option("Wireless"); 
            $opt
                ->addPro("No install wires required")
                ->addCon("Can be unreliable")
                ->addCon("Harder to debug");
            
            return $opt;
        })
        ->addOption(function(){
            $opt = new Option("Wired");
            $opt
                ->addPro("Easier to debug")
                ->addPro("More reliable")
                ->addCon("Installation hassle")
                ->addPro("Trusted by clients");

            return $opt;
        });


    $processor
        ->addSubDescision($p_cost)
        ->addSubDescision($p_size)
        ->addSubDescision($p_io)
        ->addSubDescision($p_speed)
        ->addSubDescision($p_network)
        ->addReliesOn("Water Exposure")
        ->addReliesOn("Operating Temperature");

    $p_enclosure = new Descision("Processor Enclosure");
    $p_enclosure->addReliesOn($processor);
}