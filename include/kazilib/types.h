
#pragma once

#include <iostream>
#include <string>
#include <type_traits>
#include <memory>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <filesystem>
#include <chrono>
#include <format>
#include <functional>
#include <optional>
#include <cassert>
#include <ranges>
#include <fstream>
#include <thread>
#include <mutex>

//namespace kazin
//{
using std::cout;
using std::cin;
using std::endl;


using std::string;
using std::wstring;
using std::to_string;
using namespace std::string_literals;


using i64 = std::int64_t;
using i32 = std::int32_t;
using i16 = std::int16_t;
using i8 = std::int8_t;

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8 = std::uint8_t;

using std::size_t;
using szt = size_t;


using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;


using std::vector;
using std::erase_if;
using std::erase;


using std::array;


using std::unordered_map;


//namespace chrono = std::chrono;
using namespace std::chrono;


using std::format;


using std::function;


using std::copy;
using std::copy_n;
using std::move;
using std::forward;

using std::enable_if_t;
using std::is_integral_v;
using std::is_enum_v;
using std::is_unsigned_v;
using std::is_signed_v;
using std::is_same_v;
using std::underlying_type_t;
using std::is_base_of_v;
using std::is_pointer_v;

using std::find;

using std::to_underlying;


namespace ranges = std::ranges;
namespace views = ranges::views;
using std::ranges::find_if;


using std::ifstream;
using std::ofstream;


using Path = std::filesystem::path;


namespace this_thread = std::this_thread;


using std::mutex;
using unique_mutex = std::unique_lock<mutex>;
//}

