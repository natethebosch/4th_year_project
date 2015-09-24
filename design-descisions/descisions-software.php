<?php
/**
 * @Author: Nate Bosscher
 * @Date:   2015-09-24 18:54:22
 * @Last Modified by:   Nate Bosscher
 * @Last Modified time: 2015-09-24 19:00:57
 */

$software = new Descision("Software");
{
    $s_calibration = new Descision("Calibration");
    $s_calibration
        ->addAffects("User Experience")
        ->addOption(function(){
            $opt = new Option("Upload a file");
            $opt
                ->addReliesOn("Network Capabilities")
                ->addReliesOn("User Interface");

            return $opt;
        })
        ->addOption(function(){
            $opt = new Option("USB insert");

            $opt
                ->addCon("Requires auto-run (unsafe)")
                ->addCon("Requires USB access")
                ->addReliesOn("Processor");

            return $opt;
        });
        ->addOption(function(){
            $opt = new Option("Calibration-Mode-Button-Routine");
            $opt
                ->addPro("Requires user to calibrate each unit in calibration environment (unit precision)")
                ->addCon("Requires user to calibrate each unit in calibration environment (tedious)")
                ->addCon("Additional hardware features (button)");

            return $opt;
        });

    $s_code_public_access = new Descision("Public Code Accces");
    $s_code_public_access
        ->addOption(function(){
            $opt = new Option("Yes");
            $opt
                ->addPro("Allows for advanced users to 'role their own'")
                ->addCon("Users may mess up software and ask for assistance");

            return $opt;
        })->addOption(function(){
            $opt = new Option("No");

            return $opt;
        });

    $s_language = new Descision("Programming Language");
    $s_language
        ->addReliesOn("Processor")
        ->addOption(function(){
            $opt = new Option("C");
            $opt
                ->addPro("Standard")
                ->addPro("Faster than ASM")
                ->addCon("Less versatile than c++");

            return $opt;
        })
        ->addOption(function(){
            $opt = new Option("C++");
            $opt
                ->addPro("Versatile")
                ->addPro("Faster than ASM and C");

            return $opt;
        })
        ->addOption(function(){
            $opt = new Option("ASM");
            $opt
                ->addPro("Well supported")
                ->addCon("Slow development");

            return $opt;
        });

    $su = new Descision("User Experience");
    {
        $su_admin = new Descision("Admin Access");
        $su_admin
            ->addOption(function(){
                $opt = new Option("Web based");
                $opt
                    ->addPro("Fast development for intricate UI")
                    ->addPro("Convinient")
                    ->addPro("Nice ui features")
                    ->addCon("Requires user network access");

                return $opt;
            })
            ->addOption(function(){
                $opt = new Option("Desktop Application");
                $opt
                    ->addPro("Easy for user to access")
                    ->addCon("Longer development time")
                    ->addPro("Convinient")
                    ->addCon("Many platform specific issues");

                return $opt;
            })
            ->addOption(function(){
                $opt = new Option("USB auto-download/upload");
                $opt
                    ->addCon("Requires USB access")
                    ->addCon("Requires USB auto-run (dangerous)")
                    ->addPro("Simple access");

                return $opt;
            });

        $su_data = new Descision("Data Access");
        $su_data
        	->addOption(function(){
        		$opt = new Option("Rolling network CSV file");
        		$opt
        			->addPro("Simple to implement")
        			->addPro("Simple to access");

        		return $opt;
        	})
        	->addOption(function(){
        		$opt = new Option("USB auto download");
        		$opt
        			->addCon("Requires USB access")
        			->addCon("Requires USB auto save (dangerous)")
        			->addReliesOn("Processor");

        		return $opt;
        	});

        $su
	        ->addSubDescision($su_data)
	        ->addSubDescision($su_admin);
    }

    $software
    	->addSubDescision($s_calibration)
    	->addSubDescision($s_code_public_access)
    	->addSubDescision($s_language)
    	->addSubDescision($su);
}