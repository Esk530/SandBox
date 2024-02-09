# SandBox
SandBox repository

# thrust async について

thrust は、ほとんど C++ ライクな書き方で、GPU プログラムを記述できるので便利であるが、for_each などは同期してしまう。
本来GPU が動いている間、CPU はGPU の終了を待つ必要はないので、非同期で動作するほうが嬉しい。

カーネル関数を直接記載する場合は、そのままで非同期実行してくれるが、thrust で記載する場合は、 ``async`` を使用する。
これは、 <thrsut/async/> にあるファイルから使用できるので、必要に応じて使用する。

シンプルな確認例は、以下の通り。
同期する場合は、for_each の終了を待つため、timer で計算時間が検知されるが非同期の場合は、待たないので検知されないはず。

``` cpp
    thrust::device_event e(thrust::new_stream);
    {
        po::simpleTimer timer("for_each_async");

        e = thrust::async::for_each(
            thrust::device.after(e),thrust::counting_iterator<int>(0),thrust::counting_iterator<int>(N),
            [a,b,c]__device__(int id){
                c[id] = a[id] + b[id];
            }
        );

        std::cout << "Out" << std::endl;
    }
    e.wait();

```

同様に、 thrust::for_each で記述した場合に 8ms だったが、 async で記載したものは 0ms であった
