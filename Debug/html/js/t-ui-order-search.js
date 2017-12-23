var UIOrderSearch = {
    createNew: function() {
        var wnd = {};
        //=====私有成员变量=====     
        var $m_ui_body = $('#ui_body');
        var $m_ui_table = undefined;
       
        var m_url_search_data = "order-get.zc"; //数据获取接口
        var m_url_get_condition = "order-getcondition.zc"; //展示详情接口
        var m_url_del_order = "order-del.zc";   //数据删除接口

        var m_tablecolumns =   [ { 
            "data": "id"
        },        { 
            "data": "customername" 
        },        { 
            "data": "userrname" 
        },        { 
            "data": "price"
        },        { 
            "data": "finalprice"
        },        { 
            "data": "time"
        },          {        
            "data":  null,
            "className":   "center",
            "defaultContent": '<a id="showcondition" href="#"><i class="fa fa-file-text-o"></i>重算</a>&nbsp;&nbsp;<a id="neworder" href="#"><i class="fa fa-paper-plane-o"></i>下单</a>'      
        }];

        var m_requestParam; //查询参数

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
                            <h2>总览 <small>订单</small></h2>\
                            <div class="clearfix"></div>\
                        </div>\
                        <div class="x_content">\
                            <p class="text-muted font-13 m-b-30">\
                            功能描述：此处展示了目前所有的订单信息，您可通过该页面进行修改与删除。\
                            </p>\
                            <div class="form-horizontal form-label-left">\
                                <div class="form-group">\
                                    <div class="col-md-12 col-sm-12 col-xs-12" id="search-date-div">\
                                        <input type="text" id="search-date-picker" class="form-control">\
                                        <i class="glyphicon glyphicon-calendar fa fa-calendar" id="search-date-div-btn"></i>\
                                    </div>\
                                </div>\
                            </div>\
                            <br />\
                            <table id="table_source" class="table table-striped table-bordered">\
                            <thead>\
                                <tr>\
                                <th>编号</th>\
                                <th>客户名称</th>\
                                <th>销售人员</th>\
                                <th>计算价</th>\
                                <th>订单价</th>\
                                <th>保存时间</th>\
                                <th>订单操作</th>\
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
                            <h2>生成 <small>订单</small></h2>\
                            <div class="clearfix"></div>\
                        </div>\
                        <div class="x_content">\
                            <p class="text-muted font-13 m-b-30">\
                                功能描述：您可通过此处生成订单。\
                            </p>\
                        </div>\
                    </div>\
                </div>';
            $m_ui_body.html(html_str);
            initUIControlParam();
           
            bingShowConditionEvent();
            bingDeleteEvent();
            
        }

        //更新界面参数对象
        function initUIControlParam() {
            $m_ui_table = $("#table_source");
            initTimePicker();
            //默认刷新今日数据
            m_requestParam = {
                user: $('#welcome_tip').html().trim(),
                begin: moment().format('YYYY-MM-DD') + ' 00:00:00',
                end: moment().format('YYYY-MM-DD') + ' 23:59:59'
            };
            searchhData();
        }

        function initTimePicker(){
            //绑定日期选择按钮事件
            $('#search-date-div-btn').click(function() {
                $('#search-date-picker').click();
            });

            //初始化日期选择框
            var options = {};
            options.autoApply=true;            //不用点击Apply或者应用按钮就可以直接取得选中的日期
            options.maxDate = moment();        //最大时间  
            options.showDropdowns = true;      //允许年份和月份通过下拉框的形式选择
            options.dateLimit = { days: 90 };  //起止时间的最大间隔
            options.ranges = {
                '今天': [moment(), moment()],
                '昨天': [moment().subtract(1, 'days'), moment().subtract(1, 'days')],
                '最近7天': [moment().subtract(6, 'days'), moment()],
                '最近30天': [moment().subtract(29, 'days'), moment()],
                '本月': [moment().startOf('month'), moment().endOf('month')],
                '上月': [moment().subtract(1, 'month').startOf('month'), moment().subtract(1, 'month').endOf('month')]
            };
            options.locale = {
                //direction: 'rtl',
                format: 'MM/DD/YYYY',
                separator: ' - ',
                applyLabel: '确定',
                cancelLabel: '取消',
                fromLabel: '开始日期',
                toLabel: '结束日期',
                customRangeLabel: '自定义',
                daysOfWeek: ['日', '一', '二', '三', '四', '五','六'],
                monthNames: ['一月', '二月', '三月', '四月', '五月', '六月', '七月', '八月', '九月', '十月', '十一月', '十二月'],
                firstDay: 1
            };
            $('#search-date-picker').daterangepicker(options, function(start, end, label) { 
                m_requestParam = {
                    user: $('#welcome_tip').html().trim(),
                    begin: start.format('YYYY-MM-DD HH:mm:ss'),
                    end: end.format('YYYY-MM-DD HH:mm:ss')
                };
                searchhData();
            });
        }

        //查询数据
        function searchhData() {
            $m_ui_table.dataTable({   
                destroy: true, //可重新加载
                ajax:   function (data,  callback,  settings)  {    
                    //ajax请求数据  
                    $.ajax({    
                        type: "POST",
                        url: m_url_search_data,
                        cache:  false,
                        data: JSON.stringify(m_requestParam),
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



        // function refreshSourceType() {
        //     $.ajax({    
        //         type: "POST",
        //         url: "sourcetype-all.zc",
        //         cache:  false,
        //         data: JSON.stringify("{'get':'all'}"),
        //         dataType: "json",
        //         success: function (result)  {     //封装返回数据    
        //             if (result.data.length == 0) {
        //                 $m_ui_select_sourcetype.html("");
        //                 toastr.error('暂未配置材料类别信息，请前往配置！');
        //                 return;
        //             }

        //             var strHtml = '';
        //             for (var i = 0; i < result.data.length; i++) {
        //                 var strSubHtml = '<option>' + result.data[i].name + '</option>';
        //                 strHtml = strHtml + strSubHtml;
        //             }
        //             $m_ui_select_sourcetype.html(strHtml);
        //         },
        //         error: function() {
        //             $m_ui_select_sourcetype.html("");
        //             toastr.error('获取材料类别信息失败,请刷新重试！');
        //         }  
        //     });  
        // }

        //绑定展示详情事件
        function bingShowConditionEvent(){
            $m_ui_table.on( 'click', 'a#showcondition', function (even)  {        
                even.preventDefault; //方法阻止元素发生默认的行为（例如，当点击提交按钮时阻止对表单的提交）。  
                // 得到当前对象的值                 
                var data = $m_ui_table.DataTable().row( $(this).parents('tr') ).data();
                var strParam = {
                    "id": data.id
                };         
                $.ajax({               
                    url: m_url_get_condition,
                    type: 'POST',
                    dataType: 'json',
                    data: JSON.stringify(strParam),
                    success: function(result) {   
                        if(result.ok == 0){
                            var conditionJson = JSON.parse(result.condition); 
                            if(conditionJson.source.length > 0){
                                if(conditionJson.source[0].producttype == 0){
                                    g_oReticuleCalculate.loadByParam(result.condition);         
                                }else{
                                    g_oBoxCalculate.loadByParam(result.condition);   
                                }
                            }else{
                                toastr.error('订单未选择制作材料，无法查看详情！');  
                            }
                            
                        }else{
                            toastr.error(result.errorinfo);  
                        }           
                    },
                    error: function() {              
                        toastr.error('获取详情信息失败,请检查服务器及网络后重试！');            
                    }           
                })  
            }); 
        }

        //绑定删除按钮事件
        function bingDeleteEvent() {  
            $m_ui_table.on( 'click', 'a#delrow', function (even)  {        
                even.preventDefault; //方法阻止元素发生默认的行为（例如，当点击提交按钮时阻止对表单的提交）。  
                // 得到当前对象的值                 
                var data = $m_ui_table.DataTable().row( $(this).parents('tr') ).data();  

                var textInfoStr = "确定要删除本条订单吗?";
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
                                "id": data.id
                            };         
                            $.ajax({               
                                url: m_url_del_order,
                                type: 'POST',
                                dataType: 'json',
                                data: JSON.stringify(strParam),
                                success: function(result) {   
                                    if(result.ok == 0){
                                        toastr.success('订单删除成功');
                                        searchhData();           
                                    }else{
                                        toastr.error(result.errorinfo);  
                                    }           
                                },
                                error: function() {              
                                    toastr.error('删除订单信息失败,请检查服务器及网络后重试！');            
                                }           
                            })  
                        }
                    });
            }); 
        }

        return wnd;
    }
};

var g_oOrderSearch = UIOrderSearch.createNew();