var UIPresspriceConfig = {
    createNew: function() {
        var wnd = {};
        //=====私有成员变量=====     
        var $m_ui_body = $('#ui_body');
        var $m_ui_table = undefined;
        var $m_ui_btn_add = undefined;

        var $m_ui_select_pressname = undefined;
        var $m_ui_select_presstype = undefined;
        var $m_ui_input_begin = undefined;
        var $m_ui_input_end = undefined;
        var $m_ui_input_price = undefined;

        var m_url_data = "pressprice-all.zc"; //数据获取接口
        var m_url_add = "pressprice-add.zc"; //数据新增接口
        var m_url_del = "pressprice-del.zc"; //数据删除接口
        var m_tablecolumns =   [ { 
            "data": "pressname"
        },        {    
            "data": "presstypename"
        },        {    
            "data": "range"
        },        {    
            "data": "price"
        },        {    
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
                    <h2>总览 <small>印刷价格</small></h2>\
                    <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                    <p class="text-muted font-13 m-b-30">\
                        功能描述：此处展示了目前所有的印刷价格信息，您可通过该页面进行修改与删除。\
                    </p>\
                    <table id="table_pressprice" class="table table-striped table-bordered">\
                        <thead>\
                        <tr>\
                            <th>印刷机名称</th>\
                            <th>印刷方式</th>\
                            <th>数量范围(个)</th>\
                            <th>价格(元)</th>\
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
                        <h2>新增 <small>印刷价格规则</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <p class="text-muted font-13 m-b-30">\
                            功能描述：您可通过该页面新增印刷价格规则。\
                        </p>\
                        <div class="form-horizontal form-label-left">\
                            <div class="form-group">\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">印刷机名称</label>\
                                <div class="col-md-5 col-sm-5 col-xs-12">\
                                    <select id="select_press_name" class="form-control">\
                                    </select>\
                                </div>\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">印刷方式</label>\
                                <div class="col-md-5 col-sm-5 col-xs-12">\
                                    <select id="select_press_type" class="form-control">\
                                    </select>\
                                </div>\
                            </div>\
                            <div class="form-group">\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">范围(个)</label>\
                                <div class="col-md-3 col-sm-3 col-xs-12">\
                                    <input id="input_pressprice_begin" type="text" class="form-control" placeholder="">\
                                </div>\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">~</label>\
                                <div class = "col-md-3 col-sm-3 col-xs-12" > \
                                    <input id = "input_pressprice_end" type = "text" class = "form-control" placeholder = "" > \
                                </div>\
                                <label class = "control-label col-md-1 col-sm-1 col-xs-12">价格(元)</label>\
                                <div class = "col-md-3 col-sm-3 col-xs-12" > \
                                    <input id = "input_pressprice_price" type = "text" class = "form-control" placeholder = "" > \
                                </div>\
                            </div>\
                            <div class = "ln_solid" > \
                            </div>\
                            <div class = "form-group" > \
                                <div class = "col-md-9 col-sm-9 col-xs-12 col-md-offset-1" > \
                                    <button id = "submit_add_pressprice" class = "btn btn-primary" >新增</button>\
                                </div > \
                            </div>\
                        </div > \
                    </div>\
                </div > \
            </div>';
            $m_ui_body.html(html_str);
            initUIControlParam();
            bingAddEvent();
            bingDeleteEvent();
            refreshPress();
            refreshPressType();
            refreshData();
        }

        //更新界面参数对象
        function initUIControlParam() {
            $m_ui_table = $("#table_pressprice");
            $m_ui_btn_add = $('#submit_add_pressprice');

            $m_ui_select_pressname = $('#select_press_name');
            $m_ui_select_presstype = $('#select_press_type');
            $m_ui_input_begin = $('#input_pressprice_begin');
            $m_ui_input_end = $('#input_pressprice_end');
            $m_ui_input_price = $('#input_pressprice_price');
        }

        function refreshPress() {
            $.ajax({    
                type: "POST",
                url: "press-all.zc",
                cache:  false,
                data: JSON.stringify("{'get':'all'}"),
                dataType: "json",
                success: function (result)  {     //封装返回数据    
                    if (result.data.length == 0) {
                        $m_ui_select_pressname.html("");
                        toastr.error('暂未配置印刷机信息，请前往配置！');
                        return;
                    }

                    var strHtml = '';
                    for (var i = 0; i < result.data.length; i++) {
                        var strSubHtml = '<option>' + result.data[i].name + '</option>';
                        strHtml = strHtml + strSubHtml;
                    }
                    $m_ui_select_pressname.html(strHtml);
                },
                error: function() {
                    $m_ui_select_pressname.html("");
                    toastr.error('获取印刷机信息失败,请刷新重试！');
                }  
            });  
        }

        function refreshPressType() {
            $.ajax({    
                type: "POST",
                url: "presstype-all.zc",
                cache:  false,
                data: JSON.stringify("{'get':'all'}"),
                dataType: "json",
                success: function (result)  {     //封装返回数据    
                    if (result.data.length == 0) {
                        $m_ui_select_presstype.html("");
                        toastr.error('暂未配置印刷方式信息，请前往配置！');
                        return;
                    }

                    var strHtml = '';
                    for (var i = 0; i < result.data.length; i++) {
                        var strSubHtml = '<option>' + result.data[i].name + '</option>';
                        strHtml = strHtml + strSubHtml;
                    }
                    $m_ui_select_presstype.html(strHtml);
                },
                error: function() {
                    $m_ui_select_presstype.html("");
                    toastr.error('获取印刷方式信息失败,请刷新重试！');
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
                if (isNull($m_ui_input_begin.val().trim())) {
                    toastr.error('范围起始值不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_begin.val().trim())) {
                    toastr.error('范围起始值仅能输入数字！');
                    return;
                }

                if (isNull($m_ui_input_end.val().trim())) {
                    toastr.error('范围结束值不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_end.val().trim())) {
                    toastr.error('范围结束值仅能输入数字！');
                    return;
                }

                if (isNull($m_ui_input_price.val().trim())) {
                    toastr.error('价格值不能为空！');
                    return;
                }

                if (!isPrice($m_ui_input_price.val().trim())) {
                    toastr.error('价格值输入不合法，仅能精确到小数点后两位！');
                    return;
                }

                requestParam = {
                    "pressname": $m_ui_select_pressname.val().trim(),
                    "presstypename": $m_ui_select_presstype.val().trim(),
                    "begin": parseInt($m_ui_input_begin.val().trim()),
                    "end": parseInt($m_ui_input_end.val().trim()),
                    "price": parseFloat($m_ui_input_price.val().trim()),
                };

                $.ajax({
                    type: "POST",
                    url: m_url_add,
                    contentType: "application/json;charset=utf-8",
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function(result) {
                        if (result.ok == 0) {
                            toastr.success('印刷价格规则添加成功');
                            refreshData();
                            $m_ui_input_begin.val("");
                            $m_ui_input_end.val("");
                            $m_ui_input_price.val("");

                        } else {
                            toastr.error('印刷价格规则添加失败,' + result.errorinfo);
                        }
                    },
                    error: function() {
                        toastr.error('印刷价格规则添加失败,请检查服务器及网络后重试！');
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

                var textInfoStr = "确定要删除该条记录吗?";
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
                                "pressname": data.pressname,
                                "presstypename": data.presstypename,
                                "range": data.range
                            };         
                            $.ajax({               
                                url:  m_url_del,
                                type:   'POST',
                                dataType:   'json',
                                data:  JSON.stringify(strParam),
                                success:   function(result) {              
                                    if  (result.ok == 0) {  
                                        toastr.success('印刷价格规则删除成功');           
                                        refreshData();   
                                    } 
                                    else {
                                        toastr.error('印刷价格规则删除失败,' + result.errorinfo); 
                                    }              
                                },
                                error: function() {              
                                    toastr.error('印刷方式删除失败,请检查服务器及网络后重试！');            
                                }           
                            })  
                        }
                    });
            }); 
        }

        return wnd;
    }
};

var g_oPresspriceConfig = UIPresspriceConfig.createNew();