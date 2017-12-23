var UIBoxCalculate = {
    createNew: function() {
        var wnd = {};
        //=====私有成员变量=====     
        var $m_ui_body = $('#ui_body');

        var $m_ui_select_producttype = null; //产品类型下拉框
        var $m_ui_select_sourceuser_type = null;
        var $m_ui_btn_calculate = null; //计算按钮
        var $m_ui_select_source = null; //材质下拉框
        var $m_ui_label_presstype = null; //材质标签，用于隐藏
        var $m_ui_select_presstype = null; //印刷方式下拉框
        var $m_ui_tab_parttype = null; //配件类型tab
        var $m_ui_tab_partcontent = null; //配件类型tab内容区域

        var $m_ui_table_selectpart = null; //所选配件表格
        var $m_ui_table_selectsource = null;

        var $m_ui_input_len = null;
        var $m_ui_input_width = null;
        var $m_ui_input_height = null;
        var $m_ui_input_len_x = null;
        var $m_ui_input_width_x = null;
        var $m_ui_input_height_x = null;
        var $m_ui_input_count = null;
        var $m_ui_check_is_film = null;
        var $m_ui_check_is_press = null;
        var $m_ui_check_is_tangjin = null;

        var $m_ui_btn_addsource = null;

        var m_dataSet = new Array(); //选纸配件
        var m_sourceSelect = new Array(); //选纸配件

        var $m_ui_input_customer = null;
        var $m_ui_input_finalprice = null;
        var $m_ui_btn_save = null;

        //页面参数加载时，请求的数据缓存 
        var m_param_parts = [];

        var m_tablecolumns =   [ { 
            "data": "pt"
        },        {    
            "data": "st"
        },        {    
            "data": "sut"
        },        {    
            "data": "btangjin"
        },        {    
            "data": "bfilm"
        },        {  
            "data": "bpress"
        },        {  
            "data": "presstype"
        },        {    
            "data":  null,
            "className":   "center",
            "defaultContent":   '<a id="delrow" href="#"><i class="fa fa-trash-o"></i>删除</a>'      
        }];

        //=====公共方法=====
        wnd.load = function() {
            loadUI();
        }

        wnd.loadByParam = function(param){
            loadUI();
            var paramJson = JSON.parse(param); 
            $m_ui_input_count.val(paramJson.count);
            
            //加载配件
            m_param_parts = paramJson.parts;

            for(var i=0;i<paramJson.source.length;++i){
                var subItem = {
                    pt: (paramJson.source[i].producttype == 1) ? "礼品盒(盖)" : "礼品盒(底)",
                    st: paramJson.source[i].sourcetype,
                    sut: (paramJson.source[i].sourceusetype == 0) ? "灰板" : "包纸",
                    btangjin: (paramJson.source[i].btangjin) ? "烫金" : "不烫金",
                    bfilm: (paramJson.source[i].bfilm) ? "覆膜" : "不覆膜",
                    bpress: (paramJson.source[i].bpress) ? "印刷" : "不印刷",
                    presstype: paramJson.source[i].presstype
                }

                if(paramJson.source[i].producttype == 1){
                    $m_ui_input_len.val(paramJson.source[i].l);
                    $m_ui_input_width.val(paramJson.source[i].w);
                    $m_ui_input_height.val(paramJson.source[i].h);
                }else{
                    $m_ui_input_len_x.val(paramJson.source[i].l);
                    $m_ui_input_width_x.val(paramJson.source[i].w);
                    $m_ui_input_height_x.val(paramJson.source[i].h);
                }

                m_sourceSelect.push(subItem);
            }

            $m_ui_table_selectsource.DataTable({
                destroy: true, //可重新加载
                data: m_sourceSelect,
                columns: m_tablecolumns
            });
        }

        //加载用户配置界面
        function loadUI() {
            var html_str =
                '<div class="col-md-12 col-sm-12 col-xs-12">\
                    <div class="x_panel">\
                        <div class="x_title">\
                            <h2>基础信息 <small>功能描述：通过此处输入基础参数信息。</small></h2>\
                            <div class="clearfix"></div>\
                        </div>\
                        <div class="x_content">\
                            <div class="form-horizontal form-label-left">\
                                <div class="form-group">\
                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">数量(个)</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input id="input_count" type="text" class="form-control" placeholder="">\
                                    </div>\
                                </div>\
                                <div class="form-group">\
                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">盒盖长(mm)</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input id="input_len" type="text" class="form-control" placeholder="">\
                                    </div>\
                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">盒盖宽(mm)</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input id="input_width" type="text" class="form-control" placeholder="">\
                                    </div>\
                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">盒盖高(mm)</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input id="input_height" type="text" class="form-control" placeholder="">\
                                    </div>\
                                </div>\
                                <div class="form-group">\
                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">盒底长(mm)</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input id="input_len_x" type="text" class="form-control" placeholder="">\
                                    </div>\
                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">盒底宽(mm)</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input id="input_width_x" type="text" class="form-control" placeholder="">\
                                    </div>\
                                    <label class="control-label col-md-2 col-sm-2 col-xs-12">盒底高(mm)</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input id="input_height_x" type="text" class="form-control" placeholder="">\
                                    </div>\
                                </div>\
                            </div>\
                        </div>\
                    </div>\
                </div>\
                <div class="col-md-12 col-sm-12 col-xs-12">\
                    <div class="x_panel">\
                        <div class="x_title">\
                            <h2>材料选择 <small>功能描述：通过此处选择所需材料。</small></h2>\
                            <div class="clearfix"></div>\
                        </div>\
                        <div class="x_content">\
                            <div class="form-horizontal form-label-left">\
                                <div class = "form-group" > \
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">产品类型</label>\
                                    <div class="col-md-3 col-sm-3 col-xs-12">\
                                        <select id="select_product_type" class="form-control">\
                                            <option>礼品盒(盖)</option>\
                                            <option>礼品盒(底)</option>\
                                        </select>\
                                    </div>\
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">制作材质</label>\
                                    <div class="col-md-3 col-sm-3 col-xs-12">\
                                        <select id="select_source_type" class="form-control">\
                                        </select>\
                                    </div>\
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">材料用途</label>\
                                    <div class="col-md-3 col-sm-3 col-xs-12">\
                                        <select id="select_sourceuser_type" class="form-control">\
                                            <option>灰板</option>\
                                            <option>包纸</option>\
                                        </select>\
                                    </div>\
                                </div>\
                                <div class = "form-group" > \
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">是否烫金</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input class="check_green" type="checkbox" id="check_is_tangjin" checked="" required />\
                                    </div>\
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">是否覆膜</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input class="check_green" type="checkbox" id="check_is_film" checked="" required />\
                                    </div>\
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">是否印刷</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input class="check_green" type="checkbox" id="check_is_press" checked="" required />\
                                    </div>\
                                    <label id="label_press_type" class="control-label col-md-1 col-sm-1 col-xs-12">印刷方式</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <select id="select_press_type" class="form-control">\
                                        </select>\
                                    </div>\
                                </div>\
                                <div class = "form-group" > \
                                    <div class="col-md-1 col-sm-1 col-xs-12">\
                                        <button id = "submit_addsource" class = "btn btn-success" >增加材料</button>\
                                    </div>\
                                </div>\
                                <div class = "form-group" > \
                                    <table id="selectsource_table" class="table table-striped table-bordered">\
                                        <thead>\
                                            <tr>\
                                                <th>产品类型</th>\
                                                <th>材料种类</th>\
                                                <th>材料用途</th>\
                                                <th>是否烫金</th>\
                                                <th>是否覆膜</th>\
                                                <th>是否印刷</th>\
                                                <th>印刷方式</th>\
                                                <th>操作</th>\
                                            </tr>\
                                        </thead>\
                                        <tbody>\
                                        </tbody>\
                                    </table>\
                                 </div>\
                            </div>\
                        </div>\
                    </div>\
                </div>\
                <div class="col-md-12 col-sm-12 col-xs-12">\
                    <div class="x_panel">\
                        <div class="x_title">\
                            <h2>配件选择 <small>功能描述：通过此处选择所需配件。</small></h2>\
                            <div class="clearfix"></div>\
                        </div>\
                        <div class="x_content">\
                            <div class="form-horizontal form-label-left">\
                                <div class = "form-group" > \
                                        <div class="tabbable tabs-below">\
                                            <ul class="nav nav-tabs" id="tab_part_type">\
                                                <li class="active"><a href="#测试" data-toggle="tab">Section 1</a></li>\
                                                <li><a href="#btab2" data-toggle="tab">Section 2</a></li>\
                                                <li><a href="#btab3" data-toggle="tab">Section 3</a></li>\
                                                <li><a href="#btab4" data-toggle="tab">Section 4</a></li>\
                                                <li><a href="#btab5" data-toggle="tab">Section 5</a></li>\
                                                <li><a href="#btab6" data-toggle="tab">Section 6</a></li>\
                                                <li><a href="#btab7" data-toggle="tab">Section 7</a></li>\
                                                <li><a href="#btab8" data-toggle="tab">Section 8</a></li>\
                                                <li><a href="#btab9" data-toggle="tab">Section 9</a></li>\
                                            </ul>\
                                            <div class="tab-content form-group" id="tab_part_content">\
                                            </div>\
                                        </div>\
                                    <br\>\
                                    <br\>\
                                    <table id="selectpart_table" class="table table-striped table-bordered">\
                                        <thead>\
                                            <tr>\
                                                <th>配件名称</th>\
                                                <th>配件类别</th>\
                                            </tr>\
                                        </thead>\
                                        <tbody>\
                                        </tbody>\
                                    </table>\
                                </div>\
                            </div>\
                            </div>\
                        </div>\
                    </div>\
                </div>\
                <div class="col-md-12 col-sm-12 col-xs-12">\
                    <div class="x_panel">\
                        <div class="x_title">\
                            <h2>计算结果 <small>功能描述：计算包装盒的报价。</small></h2>\
                            <div class="clearfix"></div>\
                        </div>\
                        <div class="x_content">\
                            <div class="form-horizontal form-label-left">\
                                <div class = "form-group" > \
                                    <div class = "col-md-6 col-sm-6 col-xs-12" >\
                                        <button id = "submit_count" class = "btn btn-danger" >&nbsp;&nbsp;&nbsp;&nbsp;计&nbsp;&nbsp;算&nbsp;&nbsp;&nbsp;&nbsp;</button>\
                                    </div > \
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">总价</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input id="input_total_price" type="text" class="form-control" placeholder="" readonly>\
                                    </div>\
                                    <label class="control-label col-md-1 col-sm-1 col-xs-12">单价</label>\
                                    <div class="col-md-2 col-sm-2 col-xs-12">\
                                        <input id="input_single_price" type="text" class="form-control" placeholder="" readonly>\
                                    </div>\
                                </div>\
                                <div class = "form-group" > \
                                    <div class = "col-md-12 col-sm-12 col-xs-12" >\
                                        <table id="log_table" class="table table-striped table-bordered">\
                                            <thead>\
                                                <tr>\
                                                    <th>类型</th>\
                                                    <th>日志</th>\
                                                </tr>\
                                            </thead>\
                                        </table>\
                                    </div > \
                                </div>\
                            </div>\
                        </div>\
                    </div>\
                </div>\
            </div>\
            <div class="col-md-12 col-sm-12 col-xs-12">\
                <div class="x_panel">\
                    <div class="x_title">\
                        <h2>保存订单 <small>功能描述：保存订单客户信息，用于后续查询与下单。</small></h2>\
                        <div class="clearfix"></div>\
                    </div>\
                    <div class="x_content">\
                        <div class="form-horizontal form-label-left">\
                            <div class="form-group">\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">客户名称</label>\
                                <div class="col-md-8 col-sm-8 col-xs-12">\
                                    <input id="input_customer" type="text" class="form-control" placeholder="">\
                                </div>\
                                <label class="control-label col-md-1 col-sm-1 col-xs-12">最终总价</label>\
                                <div class="col-md-2 col-sm-2 col-xs-12">\
                                    <input id="input_finalprice" type="text" class="form-control" placeholder="">\
                                </div>\
                            </div>\
                        </div>\
                        <div class = "ln_solid" > \
                        </div>\
                        <div class="form-horizontal form-label-left">\
                            <div class = "form-group" > \
                                <div class = "col-md-5 col-sm-5 col-xs-12 col-md-offset-1" >\
                                    <button id = "submit_save" class = "btn btn-danger" >&nbsp;&nbsp;&nbsp;&nbsp;保&nbsp;&nbsp;存&nbsp;&nbsp;&nbsp;&nbsp;</button>\
                                </div > \
                            </div>\
                        </div>\
                    </div > \
                </div>\
            </div>\
        </div>';
            $m_ui_body.html(html_str);
            initUIControlParam();
            refreshSourceType();
            refreshPressType();
            refreshPartType();
            bingAddSource();
            bingCalculatePrice();
            bingSaveOrder();
        }

        //更新界面参数对象
        function initUIControlParam() {
            $m_ui_btn_calculate = $('#submit_count');
            $m_ui_btn_addsource = $('#submit_addsource');

            $m_ui_select_producttype = $('#select_product_type');
            $m_ui_select_sourceuser_type = $('#select_sourceuser_type');
            $m_ui_select_source = $('#select_source_type');
            $m_ui_label_presstype = $('#label_press_type');
            $m_ui_select_presstype = $('#select_press_type');
            $m_ui_tab_parttype = $('#tab_part_type');
            $m_ui_tab_partcontent = $('#tab_part_content');
            $m_ui_table_selectpart = $('#selectpart_table');
            $m_ui_table_selectsource = $('#selectsource_table');

            $m_ui_input_len = $('#input_len');
            $m_ui_input_width = $('#input_width');
            $m_ui_input_height = $('#input_height');
            $m_ui_input_count = $('#input_count');
            $m_ui_check_is_film = $('#check_is_film');
            $m_ui_check_is_press = $('#check_is_press');

            $m_ui_input_len_x = $('#input_len_x');
            $m_ui_input_width_x = $('#input_width_x');
            $m_ui_input_height_x = $('#input_height_x');
            $m_ui_check_is_tangjin = $('#check_is_tangjin');

            $m_ui_input_customer = $('#input_customer');
            $m_ui_input_finalprice = $('#input_finalprice');
            $m_ui_btn_save = $('#submit_save');
            $m_ui_btn_save.prop('disabled', true);

            $('#check_is_press').on('ifChecked', function(event) {
                $m_ui_label_presstype.show();
                $m_ui_select_presstype.show();
            }).on('ifUnchecked', function(event) {
                $m_ui_label_presstype.hide();
                $m_ui_select_presstype.hide();
            });

            $('.check_green').iCheck({
                checkboxClass: 'icheckbox_flat-green'
            });

            $(".icheckbox_flat-green").css("margin-top", "8px");

            m_dataSet.length = 0;
            m_sourceSelect.length = 0;
        }

        //刷新配件种类选择控件
        function refreshTabdrop() {
            $('.nav-pills, .nav-tabs').tabdrop();
        }

        //刷新加载的配件元素
        function refreshPartCheckUI() {
            $('.check_aero').iCheck({
                checkboxClass: 'icheckbox_flat-aero'
            });

            $('.check_aero').on('ifChecked', function(event) {
                var subData = new Array();
                subData.push(event.target.name);
                subData.push(event.target.alt);
                m_dataSet.push(subData);
                $m_ui_table_selectpart.DataTable({
                    destroy: true, //可重新加载
                    data: m_dataSet
                });

            }).on('ifUnchecked', function(event) {
                for (var i = 0; i < m_dataSet.length; i++) {
                    if (m_dataSet[i][0] == event.target.name && m_dataSet[i][1] == event.target.alt) {
                        m_dataSet.splice(i, 1);
                        break;
                    }
                }

                $m_ui_table_selectpart.DataTable({
                    destroy: true, //可重新加载
                    data: m_dataSet
                });
            });

            //遍历刷新状态
            if(m_param_parts.length > 0)
            {
                var checks = $('.check_aero');
                checks.each(function(i,check){
                    if(m_param_parts.indexOf(check.name) > -1){
                        $(check).iCheck('check');
                    }
                });
            } 
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
                        $m_ui_select_source.html("");
                        toastr.error('暂未配置制作材质信息，请前往配置！');
                        return;
                    }

                    var strHtml = '';
                    for (var i = 0; i < result.data.length; i++) {
                        var strSubHtml = '<option>' + result.data[i].name + '</option>';
                        strHtml = strHtml + strSubHtml;
                    }
                    $m_ui_select_source.html(strHtml);
                },
                error: function() {
                    $m_ui_select_source.html("");
                    toastr.error('获取制作材质信息失败,请刷新重试！');
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

        function refreshPartType() {
            $.ajax({    
                type: "POST",
                url: "parttype-all.zc",
                cache:  false,
                data: JSON.stringify("{'get':'all'}"),
                dataType: "json",
                success: function (result)  {     //封装返回数据    
                    if (result.data.length == 0) {
                        $m_ui_tab_parttype.html("");
                        $m_ui_tab_partcontent.html("");
                        toastr.error('暂未配置配件种类信息，请前往配置！');
                        return;
                    }

                    var strPartTypeHtml = '';
                    var strPartContentHtml = '';
                    for (var i = 0; i < result.data.length; i++) {
                        if (i == 0) {
                            var strSubTypeHtml = "<li class='active'><a href='#" + result.data[i].name + "' data-toggle='tab'>" + result.data[i].name + '</a></li>';
                            var strSubContentHtml = "<div class='tab-pane active' id='" + result.data[i].name + "'></div>";

                        } else {
                            var strSubTypeHtml = "<li><a href='#" + result.data[i].name + "' data-toggle='tab'>" + result.data[i].name + '</a></li>';
                            var strSubContentHtml = "<div class='tab-pane' id='" + result.data[i].name + "'></div>";

                        }
                        strPartTypeHtml = strPartTypeHtml + strSubTypeHtml;
                        strPartContentHtml = strPartContentHtml + strSubContentHtml;
                    }
                    $m_ui_tab_parttype.html(strPartTypeHtml);
                    $m_ui_tab_partcontent.html(strPartContentHtml);


                    refreshTabdrop();
                    refreshPart();
                },
                error: function() {
                    $m_ui_tab_parttype.html("");
                    $m_ui_tab_partcontent.html("");
                    toastr.error('获取配件种类信息失败,请刷新重试！');
                }  
            });  
        }

        function refreshPart() {
            $.ajax({    
                type: "POST",
                url: "part-all.zc",
                cache:  false,
                data: JSON.stringify("{'get':'all'}"),
                dataType: "json",
                success: function (result)  {     //封装返回数据    
                    if (result.data.length == 0) {
                        toastr.error('暂未配置配件信息，可前往配置！');
                        return;
                    }

                    for (var i = 0; i < result.data.length; i++) {
                        $('#' + result.data[i].type).append("<input class='check_aero' type='checkbox' name='" + result.data[i].name + "' alt='" + result.data[i].type + "' >&nbsp;" + result.data[i].name + "&nbsp;&nbsp;&nbsp;");
                    }

                    refreshPartCheckUI();
                },
                error: function() {
                    toastr.error('获取配件信息失败,请刷新重试！');
                }  
            });  
        }

        function bingAddSource() {
            $m_ui_btn_addsource.on('click', function() {
                if($m_ui_select_source.val() == null){
                    toastr.error('暂未配置材料类型信息，请前往配置！');
                    return;
                }

                var subData = new Array();
                var bfilmTangjin,bfilmTip, bPressTip;
                var strPressType = null;
                if ($m_ui_check_is_tangjin.is(':checked')) {
                    bfilmTangjin = "烫金";
                } else {
                    bfilmTangjin = "不烫金";
                }
                if ($m_ui_check_is_film.is(':checked')) {
                    bfilmTip = "覆膜";
                } else {
                    bfilmTip = "不覆膜";
                }
                if ($m_ui_check_is_press.is(':checked')) {
                    bPressTip = "印刷";
                    strPressType = $m_ui_select_presstype.val();
                } else {
                    bPressTip = "不印刷";
                    strPressType = "-";
                }

                if(strPressType == null){
                    toastr.error('暂未配置印刷方式信息，请前往配置！');
                    return;
                }

                var subItem = {
                    pt: $m_ui_select_producttype.val().trim(),
                    st: $m_ui_select_source.val().trim(),
                    sut: $m_ui_select_sourceuser_type.val().trim(),
                    btangjin:bfilmTangjin,
                    bfilm: bfilmTip,
                    bpress: bPressTip,
                    presstype: strPressType
                }

                m_sourceSelect.push(subItem);
                $m_ui_table_selectsource.DataTable({
                    destroy: true, //可重新加载
                    data: m_sourceSelect,
                    columns: m_tablecolumns
                });

                bingDeleteEvent();
            });
        }

        //绑定删除按钮事件
        function bingDeleteEvent() {  
            $m_ui_table_selectsource.on( 'click', 'a#delrow', function (even)  {        
                even.preventDefault; //方法阻止元素发生默认的行为（例如，当点击提交按钮时阻止对表单的提交）。  
                // 得到当前对象的值                 
                var  data = $m_ui_table_selectsource.DataTable().row( $(this).parents('tr') ).data();  

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
                            for (var i = 0; i < m_sourceSelect.length; i++) {
                                if (m_sourceSelect[i].pt == data.pt &&
                                    m_sourceSelect[i].st == data.st &&
                                    m_sourceSelect[i].sut == data.sut &&
                                    m_sourceSelect[i].btangjin == data.btangjin &&
                                    m_sourceSelect[i].bfilm == data.bfilm &&
                                    m_sourceSelect[i].bpress == data.bpress &&
                                    m_sourceSelect[i].presstype == data.presstype) {
                                    m_sourceSelect.splice(i, 1);
                                    break;
                                }
                            }

                            $m_ui_table_selectsource.DataTable({
                                destroy: true, //可重新加载
                                data: m_sourceSelect,
                                columns: m_tablecolumns
                            });
                        }
                    });
            }); 
        }


        function bingCalculatePrice() {
            $m_ui_btn_calculate.on('click', function() {
                if (isNull($m_ui_input_len.val().trim())) {
                    toastr.error('盒盖长度不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_len.val().trim())) {
                    toastr.error('盒盖长度仅能输入数字！');
                    return;
                }

                if (isNull($m_ui_input_width.val().trim())) {
                    toastr.error('盒盖宽度不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_width.val().trim())) {
                    toastr.error('盒盖宽度仅能输入数字！');
                    return;
                }

                if (isNull($m_ui_input_height.val().trim())) {
                    toastr.error('盒盖高度不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_height.val().trim())) {
                    toastr.error('盒盖高度仅能输入数字！');
                    return;
                }

                if (isNull($m_ui_input_len_x.val().trim())) {
                    toastr.error('盒底长度不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_len_x.val().trim())) {
                    toastr.error('盒底长度仅能输入数字！');
                    return;
                }

                if (isNull($m_ui_input_width_x.val().trim())) {
                    toastr.error('盒底宽度不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_width_x.val().trim())) {
                    toastr.error('盒底宽度仅能输入数字！');
                    return;
                }

                if (isNull($m_ui_input_height_x.val().trim())) {
                    toastr.error('盒底高度不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_height_x.val().trim())) {
                    toastr.error('盒底高度仅能输入数字！');
                    return;
                }

                if (isNull($m_ui_input_count.val().trim())) {
                    toastr.error('数量不能为空！');
                    return;
                }

                if (!isInt($m_ui_input_count.val().trim())) {
                    toastr.error('数量仅能输入数字！');
                    return;
                }

                var sourceArray = new Array();
                for (var i = 0; i < m_sourceSelect.length; i++) {
                    var nProductType = 1;
                    if (m_sourceSelect[i].pt == "礼品盒(盖)") {
                        nProductType = 1;
                    } else if (m_sourceSelect[i].pt == "礼品盒(底)") {
                        nProductType = 2;
                    }

                    var nSourceUseType = 0;
                    if (m_sourceSelect[i].sut == "灰板") {
                        nSourceUseType = 0;
                    } else if (m_sourceSelect[i].sut == "包纸") {
                        nSourceUseType = 1;
                    }

                    var btangjin = false;
                    if (m_sourceSelect[i].btangjin == "烫金") {
                        btangjin = true;
                    }

                    var bfilm = false;
                    if (m_sourceSelect[i].bfilm == "覆膜") {
                        bfilm = true;
                    }

                    var bpress = false;
                    if (m_sourceSelect[i].bpress == "印刷") {
                        bpress = true;
                    }

                    var nLen = (nProductType == 1) ? parseInt($m_ui_input_len.val().trim()) : parseInt($m_ui_input_len_x.val().trim());
                    var nWidth = (nProductType == 1) ? parseInt($m_ui_input_width.val().trim()) : parseInt($m_ui_input_width_x.val().trim());
                    var nHeight = (nProductType == 1) ? parseInt($m_ui_input_height.val().trim()) : parseInt($m_ui_input_height_x.val().trim());

                    var subSource = {
                        l: nLen,
                        w: nWidth,
                        h: nHeight,
                        producttype: parseInt(nProductType),
                        sourcetype: m_sourceSelect[i].st.trim(),
                        sourceusetype: parseInt(nSourceUseType), //灰板、包纸
                        btangjin:btangjin,
                        bfilm: bfilm,
                        bpress: bpress,
                        presstype: m_sourceSelect[i].presstype.trim()
                    };
                    sourceArray.push(subSource);
                }

                var partsArray = new Array();
                for (var i = 0; i < m_dataSet.length; i++) {
                    partsArray.push(m_dataSet[i][0]);
                }

                var requestParam = {
                    count: parseInt($m_ui_input_count.val().trim()),
                    source: sourceArray,
                    parts: partsArray
                };

                $.ajax({    
                    type: "POST",
                    url: "calculate-price.zc",
                    cache:  false,
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function (result)  {     //封装返回数据    
                        $('#input_total_price').val(result.totalprice);
                        $('#input_single_price').val(result.price);

                        $('#log_table').DataTable({
                            "ordering": false, // 禁止排序
                            destroy: true, //可重新加载
                            data: result.log,
                            "createdRow": function(row, data, dataIndex) {    
                                if (data[0] == "错误") {
                                    $('td', row).css('color', '#f00')
                                } else if(data[0] == "需求"){
                                    $('td', row).css('color', '#8080C0')
                                } else if(data[0] == "材料"){
                                    $('td', row).css('color', '#00B271')
                                } else if(data[0] == "印刷"){
                                    $('td', row).css('color', '#479AC7') 
                                } else if(data[0] == "覆膜"){
                                    $('td', row).css('color', '#B45B3E')
                                } else if(data[0] == "压痕"){
                                    $('td', row).css('color', '#6699CC')
                                }  
                            }
                        });

                        $m_ui_btn_save.removeAttr('disabled');
                    },
                    error: function() {
                        toastr.error('向服务器请求失败,请稍后重试！');
                    }  
                });  
            });
        }

        function bingSaveOrder() {
            $m_ui_btn_save.on('click', function() {
                if (isNull($m_ui_input_customer.val().trim())) {
                    toastr.error('客户名称不能为空！');
                    return;
                }

                if (isNull($m_ui_input_finalprice.val().trim())) {
                    //toastr.error('最终总价不能为空！');
                    //return;
                    $m_ui_input_finalprice.val('0.00');
                }else{
                    if (!isPrice($m_ui_input_finalprice.val().trim())) {
                        toastr.error('最终总价输入的价格值不合法，仅能精确到小数点后两位！');
                        return;
                    }
                }

                var sourceArray = new Array();
                for (var i = 0; i < m_sourceSelect.length; i++) {
                    var nProductType = 1;
                    if (m_sourceSelect[i].pt == "礼品盒(盖)") {
                        nProductType = 1;
                    } else if (m_sourceSelect[i].pt == "礼品盒(底)") {
                        nProductType = 2;
                    }

                    var nSourceUseType = 0;
                    if (m_sourceSelect[i].sut == "灰板") {
                        nSourceUseType = 0;
                    } else if (m_sourceSelect[i].sut == "包纸") {
                        nSourceUseType = 1;
                    }

                    var btangjin = false;
                    if (m_sourceSelect[i].btangjin == "烫金") {
                        btangjin = true;
                    }

                    var bfilm = false;
                    if (m_sourceSelect[i].bfilm == "覆膜") {
                        bfilm = true;
                    }

                    var bpress = false;
                    if (m_sourceSelect[i].bpress == "印刷") {
                        bpress = true;
                    }

                    var nLen = (nProductType == 1) ? parseInt($m_ui_input_len.val().trim()) : parseInt($m_ui_input_len_x.val().trim());
                    var nWidth = (nProductType == 1) ? parseInt($m_ui_input_width.val().trim()) : parseInt($m_ui_input_width_x.val().trim());
                    var nHeight = (nProductType == 1) ? parseInt($m_ui_input_height.val().trim()) : parseInt($m_ui_input_height_x.val().trim());

                    var subSource = {
                        l: nLen,
                        w: nWidth,
                        h: nHeight,
                        producttype: parseInt(nProductType),
                        sourcetype: m_sourceSelect[i].st.trim(),
                        sourceusetype: parseInt(nSourceUseType), //灰板、包纸
                        btangjin:btangjin,
                        bfilm: bfilm,
                        bpress: bpress,
                        presstype: m_sourceSelect[i].presstype.trim()
                    };
                    sourceArray.push(subSource);
                }

                var partsArray = new Array();
                for (var i = 0; i < m_dataSet.length; i++) {
                    partsArray.push(m_dataSet[i][0]);
                }

                var conditionParam = {
                    count: parseInt($m_ui_input_count.val().trim()),
                    source: sourceArray,
                    parts: partsArray
                };

                var requestParam = {
                    customer:$m_ui_input_customer.val().trim(),
                    user: $('#welcome_tip').html().trim(),
                    price: parseFloat($('#input_total_price').val().trim()),
                    finalprice:parseFloat($m_ui_input_finalprice.val().trim()),
                    condition:conditionParam
                };

                $.ajax({    
                    type: "POST",
                    url: "order-add.zc",
                    cache:  false,
                    data: JSON.stringify(requestParam),
                    dataType: "json",
                    success: function (result)  {     //封装返回数据    
                        toastr.success('订单数据保存成功');
                        $m_ui_btn_save.prop('disabled', true);;
                    },
                    error: function() {
                        toastr.error('向服务器请求失败,请稍后重试！');
                    }  
                });  
            });
        }

        return wnd;
    }
};

var g_oBoxCalculate = UIBoxCalculate.createNew();