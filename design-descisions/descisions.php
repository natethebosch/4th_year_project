<?php
/**
 * Descisions.php
 * A list of descisions for our 4th year project
 * Use to compile a heirachy of descisions
 * Nate Bosscher (c) 2015
 */

// get the framework
include __dir__."/include.php";

$cost = new Descision("Cost");

$processor = new Descision("Processor"); // all $p_AAAA are items which pertain to the processor
{
    $p_cost = new Descision("Processor Size");
    $p_cost->addAffects($cost);

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

}
