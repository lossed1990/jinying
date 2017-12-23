#pragma once

#define I_BASE_GET_PATH				"/base-path.zc"		     // �ͻ���·����ȡ
#define I_BASE_GET_SYSINFO			"/base-sysinfo.zc"	     // �ͻ���ϵͳ������Ϣ��ȡ
#define I_BASE_GET_FILEVERSION 		"/base-fileversion.zc"	 // �ͻ�����Ҫ�ļ��汾��Ϣ��ȡ

#define I_PROCESS_GET_ALL			"/process-all.zc"          // ���н�����Ϣ��ȡ
#define I_PROCESS_OPENBYNAME		"/process-openbyname.zc"   // ��ȡ���ֶ������Ľ��� | �������ƿ�������
#define I_PROCESS_CLOSEBYID			"/process-closebyid.zc"    // ���ݽ���ID���رս���
#define I_PROCESS_OPEN		        "/process-open.zc"         // �������ƺ�·����������
#define I_PROCESS_CLOSE			    "/process-close.zc"        // �������ƹرս���
#define I_PROCESS_ISEXIST			"/process-isexist.zc"      // �����Ƿ����
#define I_PROCESS_SYSTEM            "/process-system.zc"       // ִ�п���̨����

#define I_CONFIG_GET_CONFIG_LIST	"/config-getconfiglist.zc"	  // ��ȡ�����ļ��б�
#define I_CONFIG_GET_CONFIG_CONTENT	"/config-getconfigcontent.zc" // ��ȡ�����ļ�����
#define I_CONFIG_SET_CONFIG_CONTENT	"/config-setconfigcontent.zc" // ���������ļ�����
	
#define I_FILE_FOLDER_FILES			"/file-folderfiles.zc"		// ��ȡĳ���ļ����������ļ�     
#define I_FILE_REMOVE_FILE			"/file-removefile.zc"		// ɾ��ĳ���ļ�
#define I_FILE_REPLACE_FILE			"/file-replacefile.zc"		// �滻ĳ���ļ�     
#define I_FILE_UPLOAD_FILE			"/file-uploadfile.zc"		// �ϴ��ļ� 
#define I_FILE_DOWNLOAD_FILE		"/file-downloadfile.zc"		// �����ļ� 

#define I_USER_ADD                  "/user-add.zc"		           // ����û�
#define I_USER_DEL                  "/user-del.zc"		           // ɾ���û�
#define I_USER_CHANGEPW             "/user-changepw.zc"	           // �޸��û�����
#define I_USER_ALL                  "/user-all.zc"	               // ��ȡ�����û�
		
#define I_SOURCETYPE_ADD            "/sourcetype-add.zc"	       // ��ȡ����ԭ�������
#define I_SOURCETYPE_DEL            "/sourcetype-del.zc"	       // ɾ��ԭ�������
#define I_SOURCETYPE_ALL            "/sourcetype-all.zc"	       // ��ȡ����ԭ�������
#define I_SOURCE_ADD                "/source-add.zc"	           // ��ȡ����ԭ����
#define I_SOURCE_DEL                "/source-del.zc"	           // ɾ��ԭ����
#define I_SOURCE_ALL                "/source-all.zc"	           // ��ȡ����ԭ����
														           
#define I_MORERULE_ADD              "/morerule-add.zc"	           // ��ȡ���г�������
#define I_MORERULE_DEL              "/morerule-del.zc"	           // ɾ����������
#define I_MORERULE_ALL              "/morerule-all.zc"	           // ��ȡ���г�������
														           
#define I_PRESS_ADD                 "/press-add.zc"	               // ��ȡ����ӡˢ��
#define I_PRESS_DEL                 "/press-del.zc"	               // ɾ��ӡˢ��
#define I_PRESS_ALL                 "/press-all.zc"	               // ��ȡ����ӡˢ��
								  						           
#define I_PRESSTYPE_ADD             "/presstype-add.zc"	           // ��ȡ����ӡˢ��ʽ
#define I_PRESSTYPE_DEL             "/presstype-del.zc"	           // ɾ��ӡˢ��ʽ
#define I_PRESSTYPE_ALL             "/presstype-all.zc"	           // ��ȡ����ӡˢ��ʽ
								  						           
#define I_PRESSPRICE_ADD            "/pressprice-add.zc"           // ��ȡ����ӡˢ�۸�
#define I_PRESSPRICE_DEL            "/pressprice-del.zc"           // ɾ��ӡˢ�۸�
#define I_PRESSPRICE_ALL            "/pressprice-all.zc"           // ��ȡ����ӡˢ�۸�
								  						           
#define I_DIECUTTER_ADD             "/diecutter-add.zc"	           // ��ȡ���е�ģ
#define I_DIECUTTER_DEL             "/diecutter-del.zc"	           // ɾ����ģ
#define I_DIECUTTER_ALL             "/diecutter-all.zc"	           // ��ȡ���е�ģ
#define I_DIECUTTER_SETPRICE        "/diecutter-setprice.zc"	   // ���õ�ģ�۸�
#define I_DIECUTTER_GETPRICE        "/diecutter-getprice.zc"	   // ��ȡ��ģ�۸�

#define I_IMPRESS_SETCHECKPRICE     "/impress-setcheckprice.zc"	   // ����ѹ��У���
#define I_IMPRESS_GETCHECKPRICE     "/impress-getcheckprice.zc"	   // ��ȡѹ��У���
#define I_IMPRESS_SETPRICE          "/impress-setprice.zc"	       // ����ѹ�۵���
#define I_IMPRESS_GETPRICE          "/impress-getprice.zc"	       // ��ȡѹ�۵���

#define I_FILM_SETPRICE             "/film-setprice.zc"	           // ���ø�Ĥ����
#define I_FILM_GETPRICE             "/film-getprice.zc"	           // ��ȡ��Ĥ����
#define I_FILM_MORERULE_ADD         "/film-morerule-add.zc"	       // ��ȡ���и�Ĥ��������
#define I_FILM_MORERULE_DEL         "/film-morerule-del.zc"	       // ɾ����Ĥ��������
#define I_FILM_MORERULE_ALL         "/film-morerule-all.zc"	       // ��ȡ���и�Ĥ��������

#define I_PARTTYPE_ADD              "/parttype-add.zc"	           // ��ȡ�����������
#define I_PARTTYPE_DEL              "/parttype-del.zc"	           // ɾ���������
#define I_PARTTYPE_ALL              "/parttype-all.zc"	           // ��ȡ�����������
#define I_PART_ADD                  "/part-add.zc"	               // ��ȡ�������
#define I_PART_DEL                  "/part-del.zc"	               // ɾ�����
#define I_PART_ALL                  "/part-all.zc"	               // ��ȡ�������

#define I_CALCULATE_PRICE           "/calculate-price.zc"	       // ����۸�ӿ�

#define I_ORDER_ADD                 "/order-add.zc"	               // ��Ӷ���
#define I_ORDER_GET                 "/order-get.zc"	               // ��ȡ����
#define I_ORDER_GETCONDITION        "/order-getcondition.zc"	   // ��ȡ��������
#define I_ORDER_DEL                 "/order-del.zc"	               // ɾ������