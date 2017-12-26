var UISourceConfig = {
    createNew: function() {
        var wnd = {};
        //=====私有成员变量=====     
        var $m_ui_body = $('#ui_body');
        var $m_ui_table = undefined;
        var $m_ui_btn_add = undefined;

        var $m_ui_input_sourcename = undefined;
        var $m_ui_select_sourcetype = undefined;
        var $m_ui_input_sourcelen = undefined;
        var $m_ui_input_sourcewidth = undefined;
        var $m_ui_input_sourcekezhong = undefined;
        var $m_ui_input_sourcedunjia = undefined;
        var $m_ui_select_usertype = undefined;

        var m_url_data = "source-all.zc"; //数据获取接口
        var m_url_add = "source-add.zc"; //数据新增接口
        var m_url_del = "source-del.zc"; //数据删除接口
        var m_tablecolumns =   [ { 
            "data": "name"
        },        { 
            "data": "typename" 
        },        { 
            "data": "type" 
        },        { 
            "data": "kez"
        },        { 
            "data": "dunj"
        },        { 
            "data": "usetype"
        },          {        
            "data":  null,
            "className":   "center",
            "defaultContent":   '<a id="delrow" href="#"><i class="fa fa-trash-o"></i>删除</a>'      
        }];

        //=====公共方法=====
        wnd.load = function() {
            loadUI();
        }

        //加载用户配置界面
        function loadUI() {
            var html_str =
                '<div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                <div class="x_title">\
                    <h2>总览 <small>原材料</small></h2>\
                    <div class="clearfix"></div>\
                </div>\
                <div class="x_content">\
                    <p class="text-muted font-13 m-b-30">\
                    功能描述：此处展示了目前所有的原材料信息，您可通过该页面进行修改与删除。\
                    </p>\
                    <table id="table_source" class="table table-striped table-bordered">\
                    <thead>\
                        <tr>\
                        <th>材料名称</th>\
                        <th>材料类别</th>\
                        <th>规格(mm*mm)</th>\
                        <th>克重(g/m)</th>\
                        <th>吨价(元/吨)</th>\
                        <th>用途</th>\
                        <th>操作</th>\
                        </tr>\
                    </thead>\
                    <tbody>\
                    </tbody>\
                    </table>\
                </div>\
                </div>\
            </div>\
            <div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                    <div class="x_title">\
                        <h2>新增 <small>原材料</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <p class="text-muted font-13 m-b-30">\
                            功能描述：您可通过该页面新增原材料。\
                        </p>\
                        <div class="form-horizontal form-label-left">\
                            <div class="form-group">\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">材料名称</label>\
                            <div class="col-md-5 col-sm-5 col-xs-12">\
                                <input id="input_souce_name" type="text" class="form-control" placeholder="">\
                            </div>\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">材料类别</label>\
                            <div class="col-md-5 col-sm-5 col-xs-12">\
                                <select id="select_source_type" class="form-control">\
                                </select>\
                            </div>\
                            </div>\
                            <div class="form-group">\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">长度(mm)</label>\
                            <div class="col-md-2 col-sm-2 col-xs-12">\
                                <input id="input_souce_length" type="text" class="form-control" placeholder="">\
                            </div>\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">高度(mm)</label>\
                            <div class="col-md-2 col-sm-2 col-xs-12">\
                                <input id="input_souce_width" type="text" class="form-control" placeholder="">\
                            </div>\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">克重(g/m)</label>\
                            <div class="col-md-2 col-sm-2 col-xs-12">\
                                <input id="input_souce_kezhong" type="text" class="form-control" placeholder="">\
                            </div>\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">吨价(元/吨)</label>\
                            <div class="col-md-2 col-sm-2 col-xs-12">\
                                <input id="input_souce_dunjia" type="text" class="form-control" placeholder="">\
                            </div>\
                            </div>\
                            <div class="form-group">\
                            <label class="control-label col-md-1 col-sm-1 col-xs-12">材料用途</label>\
                            <div class="col-md-11 col-sm-11 col-xs-12">\
                                <select id="select_use_type" class="form-control">\
                                <option>手提袋</option>\
                                <option>包装盒</option>\
                                </select>\
                            </div>\
                            </div>\
                            <div class="ln_solid"></div>\
                            <div class="form-group">\
                            <div class="col-md-9 col-sm-9 col-xs-12 col-md-offset-1">\
                                <button id="submit_add_source" class="btn btn-primary">新增</button>\
                                </div>\
                            </div>\
                        </div>\
                    </div>\
                </div>\
            </div>';
            $m_ui_body.html(html_str);
            initUIControlParam();
            bingAddEvent();
            bingDeleteEvent();
            refreshSourceType();
            refreshData();
        }

        //更新界面参数对象
        function initUIControlParam() {
            $m_ui_table = $("#table_source");
            $m_ui_btn_add = $('#submit_add_source');
            $m_ui_input_sourcename = $('#input_souce_name');
            $m_ui_select_sourcetype = $('#select_source_type');
            $m_ui_input_sourcelen = $('#input_souce_length');
            $m_ui_input_sourcewidth = $('#input_souce_width');
            $m_ui_input_sourcekezhong = $('#input_souce_kezhong');
            $m_ui_input_sourcedunjia = $('#input_souce_dunjia');
            $m_ui_select_usertype = $('#select_use_type');
        }

        function refreshSourceType() {
            $.ajax({    
                type: "POST",
                url: "sourcetype-all.zc",
                cache:  false,
                data: JSON.stringify("{'get':'all'}"),
                dataType: "json",
                success: function (result)  {     //封装返回数据    
                    if (result.data.length == 0) {
                        $m_ui_select_sourcetype.html("");
                        toastr.error('暂未配置材料类别信息，请前往配置！');
                        return;
                    }

                    var strHtml = '';
                    for (var i = 0; i < result.data.length; i++) {
                        var strSubHtml = '<option>' + result.data[i].name + '</option>';
                        strHtml = strHtml + strSubHtml;
                    }
                    $m_ui_select_sourcetype.html(strHtml);
                },
                error: function() {
                    $m_ui_select_sourcetype.html("");
                    toastr.error('获取材料类别信息失败,请刷新重试！');
                }  
            });  
        }

        //刷新用户数据
        function refreshData() {
            $m_ui_table.dataTable({   
                destroy: true, //可重新加载
                ajax:   function (data,  callback,  settings)  {    
                    //ajax请求数据  
                    $.ajax({    
                        type: "POST",
                        url: m_url_data,
                        cache:  false,
                        //禁用缓存  
                        data: JSON.stringify("{'get':'all'}"),
                        //传入组装的参数  
                        dataType: "json",
                        success: function (result)  {     //封装返回数据    
                            var  returnData  =   {};    
                            returnData.draw  =  data.draw; //这里直接自行返回了draw计数器,应该由后台返回       
                            returnData.recordsTotal  =  result.recordsTotal; //返回数据全部记录      
                            returnData.recordsFiltered  =  result.recordsFiltered; //后台不实现过滤功能，每次查询均视作全部结果     
                            returnData.data  =  result.data; //返回的数据列表  
                            callback(returnData);    
                        }  
                    });  
                },
                columns:  m_tablecolumns         
            });
        }

        //绑定新增按钮事件
        function bingAddEvent() {
            $m_ui_btn_add.on('click', function() {
                var requestParam;
                if (isNull($m_ui_input_sourcename.val().trim())) {
                    toastr.error('新增材料名不能为空！');
                    return;
                }

                if (isNull($m_ui_input_sourcelen.val().trim())) {
                    toastr.error('新增材料长度不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_sourcelen.val().trim())) {
                    toastr.error('材料长度仅能输入数字！');
                    return;
                }

                if (isNull($m_ui_input_sourcewidth.val().trim())) {
                    toastr.error('新增材料宽度不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_sourcewidth.val().trim())) {
                    toastr.error('材料宽度仅能输入数字！');
                    return;
                }

                if (parseInt($m_ui_input_sourcelen.val().trim()) < parseInt($m_ui_input_sourcewidth.val().trim())) {
                    toastr.error('材料长度不能小于宽度值！');
                    return;
                }

                if (isNull($m_ui_input_sourcekezhong.val().trim())) {
                    toastr.error('新增材料克重不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_sourcekezhong.val().trim())) {
                    toastr.error('材料克重仅能输入数字！');
                    return;
                }

                if (isNull($m_ui_input_sourcedunjia.val().trim())) {
                    toastr.error('新增材料吨价不能为空！');
                    return;
                }

                if (!isPrice($m_ui_input_sourcedunjia.val().trim())) {
                    toastr.error('材料吨价输入不合法，仅能精确到小数点后两位！');
                    return;
                }

                var chType = $m_ui_select_usertype.val().trim();
                if (chType == "手提袋") {
                    requestParam = {
                        "name": $m_ui_input_sourcename.val().trim(),
                        "typename": $m_ui_select_sourcetype.val().trim(),
                        "l": parseInt($m_ui_input_sourcelen.val().trim()),
                        "w": parseInt($m_ui_input_sourcewidth.val().trim()),
                        "kez": parseInt($m_ui_input_sourcekezhong.val().trim()),
                        "dunj": parseFloat($m_ui_input_sourcedunjia.val().trim()),
                        "usetype": 0
                    };
                } else if (chType == "包装盒") {
                    requestParam = {
                        "name": $m_ui_input_sourcename.val().trim(),
                        "typename": $m_ui_select_sourcetype.val().trim(),
                        "l": parseInt($m_ui_input_sourcelen.val().trim()),
                        "w": parseInt($m_ui_input_sourcewidth.val().trim()),
                        "kez": parseInt($m_ui_input_sourcekezhong.val().trim()),
                        "dunj": parseFloat($m_ui_input_sourcedunjia.val().trim()),
                        "usetype": 1
                    };
                } else {
                    toastr.error('请选择材料用途！');
                    return;
                }

                $.ajax({
                    type: "POST",
                    url: m_url_add,
                    contentType: "application/json;charset=utf-8",
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function(result) {
                        if (result.ok == 0) {
                            toastr.success('材料[' + $m_ui_input_sourcename.val().trim() + ']添加成功');
                            refreshData();
                            $m_ui_input_sourcename.val("");
                            $m_ui_input_sourcelen.val("");
                            $m_ui_input_sourcewidth.val("");
                            $m_ui_input_sourcekezhong.val("");
                            $m_ui_input_sourcedunjia.val("");
                        } else {
                            toastr.error('材料[' + $m_ui_input_sourcename.val().trim() + ']添加失败,' + result.errorinfo);
                        }
                    },
                    error: function() {
                        toastr.error('材料添加失败,请检查服务器及网络后重试！');
                    }
                });
            });
        }

        //绑定删除按钮事件
        function bingDeleteEvent() {  
            $m_ui_table.on( 'click', 'a#delrow', function (even)  {        
                even.preventDefault; //方法阻止元素发生默认的行为（例如，当点击提交按钮时阻止对表单的提交）。  
                // 得到当前对象的值                 
                var  data = $m_ui_table.DataTable().row( $(this).parents('tr') ).data();  

                var textInfoStr = "确定要删除材料[" + data.name + "]吗?";
                swal({  
                        title: "提示",
                        text: textInfoStr,
                        type: "warning",
                        showCancelButton: true,
                        confirmButtonColor: "#DD6B55",
                        confirmButtonText: "确定删除",
                        cancelButtonText: "取消"
                    },
                    function(isConfirm) {  
                        if (isConfirm) {           
                            var strParam = {
                                "name": data.name
                            };         
                            $.ajax({               
                                url:  m_url_del,
                                type:   'POST',
                                dataType:   'json',
                                data:  JSON.stringify(strParam),
                                success:   function(result) {              
                                    if  (result.ok == 0) {  
                                        toastr.success('材料[' + data.name + ']删除成功');           
                                        refreshData();   
                                    } 
                                    else {
                                        toastr.error('材料[' + data.name + ']删除失败,' + result.errorinfo); 
                                    }              
                                },
                                error: function() {              
                                    toastr.error('材料删除失败,请检查服务器及网络后重试！');            
                                }           
                            })  
                        }
                    });
            }); 
        }

        return wnd;
    }
};

var g_oSourceConfig = UISourceConfig.createNew();