#pragma once

#define I_BASE_GET_PATH				"/base-path.zc"		     // 客户端路径获取
#define I_BASE_GET_SYSINFO			"/base-sysinfo.zc"	     // 客户端系统基本信息获取
#define I_BASE_GET_FILEVERSION 		"/base-fileversion.zc"	 // 客户端重要文件版本信息获取

#define I_PROCESS_GET_ALL			"/process-all.zc"          // 所有进程信息获取
#define I_PROCESS_OPENBYNAME		"/process-openbyname.zc"   // 获取可手动开启的进程 | 根据名称开启进程
#define I_PROCESS_CLOSEBYID			"/process-closebyid.zc"    // 根据进程ID，关闭进程
#define I_PROCESS_OPEN		        "/process-open.zc"         // 根据名称和路径开启进程
#define I_PROCESS_CLOSE			    "/process-close.zc"        // 根据名称关闭进程
#define I_PROCESS_ISEXIST			"/process-isexist.zc"      // 进程是否存在
#define I_PROCESS_SYSTEM            "/process-system.zc"       // 执行控制台命令

#define I_CONFIG_GET_CONFIG_LIST	"/config-getconfiglist.zc"	  // 获取配置文件列表
#define I_CONFIG_GET_CONFIG_CONTENT	"/config-getconfigcontent.zc" // 获取配置文件内容
#define I_CONFIG_SET_CONFIG_CONTENT	"/config-setconfigcontent.zc" // 设置配置文件内容
	
#define I_FILE_FOLDER_FILES			"/file-folderfiles.zc"		// 获取某个文件夹下所有文件     
#define I_FILE_REMOVE_FILE			"/file-removefile.zc"		// 删除某个文件
#define I_FILE_REPLACE_FILE			"/file-replacefile.zc"		// 替换某个文件     
#define I_FILE_UPLOAD_FILE			"/file-uploadfile.zc"		// 上传文件 
#define I_FILE_DOWNLOAD_FILE		"/file-downloadfile.zc"		// 下载文件 

#define I_USER_ADD                  "/user-add.zc"		           // 添加用户
#define I_USER_DEL                  "/user-del.zc"		           // 删除用户
#define I_USER_CHANGEPW             "/user-changepw.zc"	           // 修改用户密码
#define I_USER_ALL                  "/user-all.zc"	               // 获取所有用户
		
#define I_SOURCETYPE_ADD            "/sourcetype-add.zc"	       // 获取所有原材料类别
#define I_SOURCETYPE_DEL            "/sourcetype-del.zc"	       // 删除原材料类别
#define I_SOURCETYPE_ALL            "/sourcetype-all.zc"	       // 获取所有原材料类别
#define I_SOURCE_ADD                "/source-add.zc"	           // 获取所有原材料
#define I_SOURCE_DEL                "/source-del.zc"	           // 删除原材料
#define I_SOURCE_ALL                "/source-all.zc"	           // 获取所有原材料
														           
#define I_MORERULE_ADD              "/morerule-add.zc"	           // 获取所有出数规则
#define I_MORERULE_DEL              "/morerule-del.zc"	           // 删除出数规则
#define I_MORERULE_ALL              "/morerule-all.zc"	           // 获取所有出数规则
														           
#define I_PRESS_ADD                 "/press-add.zc"	               // 获取所有印刷机
#define I_PRESS_DEL                 "/press-del.zc"	               // 删除印刷机
#define I_PRESS_ALL                 "/press-all.zc"	               // 获取所有印刷机
								  						           
#define I_PRESSTYPE_ADD             "/presstype-add.zc"	           // 获取所有印刷方式
#define I_PRESSTYPE_DEL             "/presstype-del.zc"	           // 删除印刷方式
#define I_PRESSTYPE_ALL             "/presstype-all.zc"	           // 获取所有印刷方式
								  						           
#define I_PRESSPRICE_ADD            "/pressprice-add.zc"           // 获取所有印刷价格
#define I_PRESSPRICE_DEL            "/pressprice-del.zc"           // 删除印刷价格
#define I_PRESSPRICE_ALL            "/pressprice-all.zc"           // 获取所有印刷价格
								  						           
#define I_DIECUTTER_ADD             "/diecutter-add.zc"	           // 获取所有刀模
#define I_DIECUTTER_DEL             "/diecutter-del.zc"	           // 删除刀模
#define I_DIECUTTER_ALL             "/diecutter-all.zc"	           // 获取所有刀模
#define I_DIECUTTER_SETPRICE        "/diecutter-setprice.zc"	   // 设置刀模价格
#define I_DIECUTTER_GETPRICE        "/diecutter-getprice.zc"	   // 获取刀模价格

#define I_IMPRESS_SETCHECKPRICE     "/impress-setcheckprice.zc"	   // 设置压痕校板费
#define I_IMPRESS_GETCHECKPRICE     "/impress-getcheckprice.zc"	   // 获取压痕校板费
#define I_IMPRESS_SETPRICE          "/impress-setprice.zc"	       // 设置压痕单价
#define I_IMPRESS_GETPRICE          "/impress-getprice.zc"	       // 获取压痕单价

#define I_FILM_SETPRICE             "/film-setprice.zc"	           // 设置覆膜单价
#define I_FILM_GETPRICE             "/film-getprice.zc"	           // 获取覆膜单价
#define I_FILM_MORERULE_ADD         "/film-morerule-add.zc"	       // 获取所有覆膜出数规则
#define I_FILM_MORERULE_DEL         "/film-morerule-del.zc"	       // 删除覆膜出数规则
#define I_FILM_MORERULE_ALL         "/film-morerule-all.zc"	       // 获取所有覆膜出数规则

#define I_PARTTYPE_ADD              "/parttype-add.zc"	           // 获取所有配件种类
#define I_PARTTYPE_DEL              "/parttype-del.zc"	           // 删除配件种类
#define I_PARTTYPE_ALL              "/parttype-all.zc"	           // 获取所有配件种类
#define I_PART_ADD                  "/part-add.zc"	               // 获取所有配件
#define I_PART_DEL                  "/part-del.zc"	               // 删除配件
#define I_PART_ALL                  "/part-all.zc"	               // 获取所有配件

#define I_CALCULATE_PRICE           "/calculate-price.zc"	       // 计算价格接口

#define I_ORDER_ADD                 "/order-add.zc"	               // 添加订单
#define I_ORDER_GET                 "/order-get.zc"	               // 获取订单
#define I_ORDER_GETCONDITION        "/order-getcondition.zc"	   // 获取订单条件
#define I_ORDER_DEL                 "/order-del.zc"	               // 删除订单