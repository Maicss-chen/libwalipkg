#include <iostream>
#include <cstring>

#include "../src/PackageKit.h"
#include "../src/log.h"
int main() {

    PackageKitMM::PackageKit p;

    //设置进度更新时的回调函数
    p.setProgressCallback([](PackageKitMM::PkProgress progress, PackageKitMM::TaskType type){
        std::string str;
        if(type == PackageKitMM::TASK_FIND_PACKAGE)
            str = "查找进度：";
        else if(type == PackageKitMM::TASK_INSTALL_PACKAGE)
            str = "安装进度：";
        else if(type == PackageKitMM::TASK_REMOVE_PACKAGE)
            str = "卸载进度：";
        else if(type == PackageKitMM::TASK_REFRESH_CACHE)
            str = "更新缓存：";
        std::cout<<"\r"<<str<<progress.get_percentage()<<"% speed:"<<progress.get_speed()<<"Mb/s"<<std::flush;
        ;
//        <<"剩余时间："<<progress.get_remaining_time()<<std::flush;
//        <<"包："<<progress.get_package().get_package_name()<< std::flush; //flush作用是及时输出（清楚缓冲区）
    });

    auto repos = p.get_repo_list();
    for(auto repo : repos){
        log(repo.get_repo_id());
    }

    //更新缓存
    p.refresh_cache(false);

    if(!p.error().empty()){
        return 0;
    }

    //生成一个要查找包的包名列表
    std::vector<std::string> values;
    values.emplace_back("gimp");

    //获取查找结果
    std::cout<<"\n正在搜索...\n";
    auto pkg_list = p.find_packages_based_on_names(values);

    //筛选
    std::vector<PackageKitMM::PkPackage> install_list;
    for(auto pkg : pkg_list){
        //将架构为x86_64的未安装的软件包加入安装列表
        if(std::strcmp(pkg.get_package_arch().c_str(),"amd64") == 0 && !pkg.is_installed()){
            install_list.emplace_back(pkg);
        }
    }

    //根据筛选结果提示和安装
    if(install_list.empty()){
        std::cout<<"\n您没有任何需要安装的软件包。\n";
    } else{
        std::cout<<"\n将安装以下软件包：\n";
        for(auto pkg : install_list){
            std::cout<<pkg.get_package_id()<<"\n\t"<<pkg.get_package_summary()<<std::endl;
        }
        std::cout<<"开始安装...\n";
        //安装所有软件包
        p.install_packages(install_list);
//        p.remove_packages(install_list);
        std::cout<<"\n安装完成"<<std::endl;
    }
    return 0;
}
