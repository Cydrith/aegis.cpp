﻿//
// aegis.hpp
// aegis.cpp
//
// Copyright (c) 2017 Sara W (sara at xandium dot net)
//
// This file is part of aegis.cpp .
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include "aegis/config.hpp"
#include "aegis/ratelimit.hpp"
#include "aegis/error.hpp"
#include "aegis/utility.hpp"
#include "aegis/snowflake.hpp"
#include <optional>
#include <memory>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>


namespace aegiscpp
{

using namespace std::chrono;
namespace spd = spdlog;
using json = nlohmann::json;
using namespace std::literals;

using namespace utility;
using namespace rest_limits;

struct typing_start;
struct message_create;
struct message_update;
struct message_delete;
struct ready;
struct resumed;
struct presence_update;
struct channel_create;
struct channel_delete;
struct channel_pins_update;
struct channel_update;
struct guild_ban_add;
struct guild_ban_remove;
struct guild_create;
struct guild_delete;
struct guild_emojis_update;
struct guild_integrations_update;
struct guild_member_add;
struct guild_member_remove;
struct guild_member_update;
struct guild_members_chunk;
struct guild_role_create;
struct guild_role_delete;
struct guild_role_update;
struct guild_update;
struct message_delete_bulk;
struct message_reaction_add;
struct message_reaction_remove;
struct message_reaction_remove_all;
struct user_update;
struct voice_server_update;
struct voice_state_update;
struct webhooks_update;

class shard;
class member;
class channel;
class guild;


struct callbacks
{
    using c_inject = std::function<void(const nlohmann::json & msg, shard * _shard, aegis & bot)>;
    /// User callbacks
    std::function<void(typing_start obj)> i_typing_start;
    std::function<void(message_create obj)> i_message_create;
    std::function<void(message_create obj)> i_message_create_dm;
    std::function<void(message_update obj)> i_message_update;
    std::function<void(message_delete obj)> i_message_delete;
    c_inject i_message_delete_bulk;//TODO
    std::function<void(guild_create obj)> i_guild_create;
    std::function<void(guild_update obj)> i_guild_update;
    std::function<void(guild_delete obj)> i_guild_delete;
    std::function<void(user_update obj)> i_user_update;
    std::function<void(ready obj)> i_ready;
    std::function<void(resumed obj)> i_resumed;
    c_inject i_channel_create;//TODO
    c_inject i_channel_update;//TODO
    c_inject i_channel_delete;//TODO
    std::function<void(guild_ban_add obj)> i_guild_ban_add;
    std::function<void(guild_ban_remove obj)> i_guild_ban_remove;
    c_inject i_guild_emojis_update;//TODO
    c_inject i_guild_integrations_update;//TODO
    std::function<void(guild_member_add obj)> i_guild_member_add;
    std::function<void(guild_member_remove obj)> i_guild_member_remove;
    std::function<void(guild_member_update obj)> i_guild_member_update;
    std::function<void(guild_members_chunk obj)> i_guild_member_chunk;
    c_inject i_guild_role_create;//TODO
    c_inject i_guild_role_update;//TODO
    c_inject i_guild_role_delete;//TODO
    std::function<void(presence_update obj)> i_presence_update;
    c_inject i_voice_state_update;//TODO
    c_inject i_voice_server_update;//TODO
};


class aegis
{
public:
    /// Type of a pointer to the asio io_service
    using io_service_ptr = asio::io_service *;

    /// Type of a pointer to the Websocket++ client
    using websocket = websocketpp::client<websocketpp::config::asio_tls_client>;

    /// Type of a pointer to the Websocket++ TLS connection
    using connection_ptr= websocketpp::client<websocketpp::config::asio_tls_client>::connection_type::ptr;

    /// Type of a pointer to the Websocket++ message payload
    using message_ptr = websocketpp::config::asio_client::message_type::ptr;

    /// Type of a shared pointer to an io_service work object
    using work_ptr = std::shared_ptr<asio::io_service::work>;

    /// Constructs the aegis object that tracks all of the shards, guilds, channels, and members
    /**
    * @see shard
    * @see guild
    * @see channel
    * @see member
    *
    * @param token A string of the authentication token
    */
    aegis(std::string_view _token)
        : shard_max_count(0)
        , force_shard_count(0)
        , selfbot(false)
        , debugmode(false)
        , member_id(0)
        , mfa_enabled(false)
        , discriminator(0)
        , token{ _token }
        , status{ Uninitialized }
        , ratelimit_o{ std::bind(&aegis::call, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) }
    {
        //spdlog::set_async_mode(64);

// #ifdef _DEBUG
//         std::vector<spdlog::sink_ptr> sinks;
// #ifdef _WIN32
//         sinks.push_back(std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>());
// #else //ansi terminal colors
//         sinks.push_back(std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());
// #endif
//         sinks.push_back(std::make_shared<spdlog::sinks::stderr_sink_mt>());
// 
//         log = spdlog::create("aegis", begin(sinks), end(sinks));
// 
// #else
        log = spdlog::stdout_color_mt("aegis");
//#endif
        log->set_pattern("%Y-%m-%d %H:%M:%S.%e [%L] [th#%t] : %v");
        log->set_level(spdlog::level::level_enum::trace);
        ratelimit_o.add(bucket_type::GUILD);
        ratelimit_o.add(bucket_type::CHANNEL);
        ratelimit_o.add(bucket_type::EMOJI);
    }

    /// Destroys the shards, stops the asio::work object, destroys the websocket object, and attempts to join the rest_thread thread
    ///
    ~aegis()
    {
    }

    aegis(const aegis &) = delete;
    aegis(aegis &&) = delete;
    aegis & operator=(const aegis &) = delete;

    /// Perform basic initialization of the websocket object using the user-constructed asio::io_service
    /**
    * @param ptr A string of the uri path to get
    *
    * @param ec The error_code out value
    */
    void initialize(io_service_ptr ptr, std::error_code & ec)
    {
        log->info("Initializing");
        if (status != Uninitialized)
        {
            log->critical("aegis::initialize() called in the wrong state");
            using error::make_error_code;
            ec = make_error_code(error::invalid_state);
            return;
        }

        log->debug("aegis::initialize()");
        websocket_o.init_asio(ptr, ec);
        if (ec)
            return;
        websocket_o.start_perpetual();
        status = Ready;
        ec.clear();
    }

    /// Perform basic initialization of the websocket object using the user-constructed asio::io_service
    /**
    * @see initialize(io_service_ptr, std::error_code&)
    *
    * @param ptr Pointer to a user-owned asio::io_service object pointer
    */
    void initialize(io_service_ptr ptr)
    {
        std::error_code ec;
        initialize(ptr, ec);
        if (ec)
            throw std::system_error(ec);
    }

    /// Perform basic initialization of the asio::io_service and websocket objects
    /**
    * @param ec The error_code out value
    */
    void initialize(std::error_code & ec)
    {
        log->info("Initializing");
        if (status != Uninitialized)
        {
            log->critical("aegis::initialize() called in the wrong state");
            using error::make_error_code;
            ec = make_error_code(error::invalid_state);
            return;
        }

        log->debug("aegis::initialize()");
        websocket_o.init_asio(ec);
        if (ec)
            return;
        websocket_o.start_perpetual();
        status = Ready;
        ec.clear();
    }

    /// Perform basic initialization of the asio::io_service and websocket objects
    /**
    * @see initialize(std::error_code&)
    */
    void initialize()
    {
        std::error_code ec;
        initialize(ec);
    }

    /// Performs a basic startup sequence for initializing and connecting the library
    ///
    void easy_start()
    {
        std::error_code ec;
        // Pass our io_service object to bot to initialize
        initialize(ec);
        if (ec) { log->error("Initialize fail: {}", ec.message()); shutdown();  return; }
        // Create our websocket connection
        create_websocket(ec);
        if (ec) { log->error("Websocket fail: {}", ec.message()); shutdown();  return; }
        // Connect the websocket[s]
        starttime = std::chrono::steady_clock::now();
        std::thread make_connections([&]
        {
            connect(ec);
            if (ec) { log->error("Connect fail: {}", ec.message()); shutdown(); return; }
        });
        // Run the bot
        run();
        make_connections.join();
    }

    /// Invokes a shutdown on the entire lib. Sets internal state to `Shutdown`, stops the asio::work object
    /// and propogates the Shutdown state along with closing all websockets within the shard vector
    AEGIS_DECL void shutdown();

    /// Creates the parent websocket object
    /**
    * @param ec The error_code out value
    */
    AEGIS_DECL void create_websocket(std::error_code & ec);

    /// Get the internal (or external) io_service object
    asio::io_service & io_service()
    {
        return websocket_o.get_io_service();
    }

    /// Get the internal (or external) io_service object
    asio::io_service & rest_service()
    {
        return rest_scheduler;
    }

    /// Initiate websocket connection
    /**
    * @param ec The error_code out value
    */
    AEGIS_DECL void connect(std::error_code & ec) noexcept;

    /// Assign the message, connect, and close callbacks to the websocket object
    /**
    * @param _shard The shard object this websocket belong to
    */
    AEGIS_DECL void setup_callbacks(shard * _shard);

    /// Outputs the last 5 messages received from the gateway
    ///
    AEGIS_DECL void debug_trace(shard * _shard);

    /// Performs a GET request on the path
    /**
    * @see rest_reply
    * @param path A string of the uri path to get
    * 
    * @returns Response object
    */
    AEGIS_DECL std::optional<rest_reply> get(std::string_view path);

    /// Performs a GET request on the path with content as the request body
    /**
    * @see rest_reply
    * @param path A string of the uri path to get
    * 
    * @param content JSON formatted string to send as the body
    *
    * @returns Response object
    */
    AEGIS_DECL std::optional<rest_reply> get(std::string_view path, std::string_view content);

    /// Performs a GET request on the path with content as the request body
    /**
    * @see rest_reply
    * @param path A string of the uri path to get
    *
    * @param content JSON formatted string to send as the body
    *
    * @returns Response object
    */
    AEGIS_DECL std::optional<rest_reply> post(std::string_view path, std::string_view content);

    /// Performs an HTTP request on the path with content as the request body using the method method
    /**
    * @see rest_reply
    * @param path A string of the uri path to get
    *
    * @param content JSON formatted string to send as the body
    *
    * @param method The HTTP method of the request
    *
    * @returns Response object
    */
    AEGIS_DECL std::optional<rest_reply> call(std::string_view path, std::string_view content, std::string_view method);

    /// Spawns specified amount of threads and starts running the io_service or default hardware hinted contexts
    /**
    * @param count Spawn `count` threads and run io_service object
    */
    void run(std::size_t count = 0)
    {
        if (count == 0)
            count = std::thread::hardware_concurrency();

        // Create rest_scheduler work object so threads do not exit immediately
        rest_work = std::make_shared<asio::io_service::work>(std::ref(rest_scheduler));
     
        // Create a pool of threads to run all of the io_services.
        std::vector<std::thread> threads;
        for (std::size_t i = 0; i < count; ++i)
            threads.emplace_back(std::bind(static_cast<asio::io_service::count_type(asio::io_service::*)()>(&asio::io_service::run), &io_service()));

        // Create a pool of threads to run the rest scheduler.
        std::vector<std::thread> rest_threads;
        for (std::size_t i = 0; i < count; ++i)
            rest_threads.emplace_back(std::bind(static_cast<asio::io_service::count_type(asio::io_service::*)()>(&asio::io_service::run), &rest_service()));

        for (auto & thread : threads)
            thread.join();

        for (auto & rest_thread : rest_threads)
            rest_thread.join();
    }

    /**
    * Yields operation of the current thread until library shutdown is detected
    */
    void yield() const noexcept
    {
        while (status != Shutdown)
        {
            std::this_thread::yield();
        }
    }

    ratelimiter & ratelimit() noexcept { return ratelimit_o; }
    websocket & get_websocket() noexcept { return websocket_o; }
    bot_status get_state() const noexcept { return status; }
    void set_state(bot_status s) noexcept { status = s; }

    /// Temporary until I implement strand executors
    asio::io_service rest_scheduler;

    uint32_t shard_max_count;

    std::string ws_gateway;

    std::vector<std::unique_ptr<shard>> shards;
    std::map<snowflake, std::unique_ptr<member>> members;
    std::map<snowflake, std::unique_ptr<channel>> channels;
    std::map<snowflake, std::unique_ptr<guild>> guilds;
    std::map<std::string, uint64_t> message_count;

    json self_presence;
    int16_t force_shard_count;
    bool selfbot;
    bool debugmode;
    std::string mention;
    callbacks _callbacks;
    bool wsdbg = false;

    AEGIS_DECL member * get_member(snowflake id) const noexcept;

    AEGIS_DECL channel * get_channel(snowflake id) const noexcept;

    AEGIS_DECL guild * get_guild(snowflake id) const noexcept;

    AEGIS_DECL member * get_member_create(snowflake id) noexcept;

    AEGIS_DECL channel * get_channel_create(snowflake id) noexcept;

    AEGIS_DECL guild * get_guild_create(snowflake id, shard * _shard) noexcept;

    AEGIS_DECL member & get_member_by_any(snowflake id) const;

    //called by CHANNEL_CREATE (DirectMessage)
    AEGIS_DECL void channel_create(const json & obj, shard * _shard);
    AEGIS_DECL void rich_presence(const json & obj, shard * _shard);
    AEGIS_DECL void status_thread();

    member * self() const
    {
        if (_self == nullptr)
            throw aegiscpp::exception("Self not found", make_error_code(error::member_not_found));
        return _self;
        /*auto it = members.find(state.user.id);
        if (it == members.end())
            return nullptr;
        return it->second.get();*/
    }

    AEGIS_DECL int64_t get_member_count() const noexcept;

    std::shared_ptr<spdlog::logger> log;

    std::chrono::steady_clock::time_point starttime;
    std::string uptime() const noexcept
    {
        using seconds = std::chrono::duration<int, std::ratio<1, 1>>;
        using minutes = std::chrono::duration<int, std::ratio<60, 1>>;
        using hours = std::chrono::duration<int, std::ratio<3600, 1>>;
        using days = std::chrono::duration<int, std::ratio<24 * 3600, 1>>;

        std::stringstream ss;
        std::chrono::time_point<std::chrono::steady_clock> now_t = std::chrono::steady_clock::now();

        auto time_is = now_t - starttime;
        auto d = std::chrono::duration_cast<days>(time_is).count();
        time_is -= days(d);
        auto h = std::chrono::duration_cast<hours>(time_is).count();
        time_is -= hours(h);
        auto m = std::chrono::duration_cast<minutes>(time_is).count();
        time_is -= minutes(m);
        auto s = std::chrono::duration_cast<seconds>(time_is).count();

        if (d)
            ss << d << "d ";
        if (h)
            ss << h << "h ";
        if (m)
            ss << m << "m ";
        ss << s << "s ";
        return ss.str();
    }

private:
    friend class guild;
    friend class channel;
    friend class shard;

    snowflake member_id;
    std::string username;
    bool mfa_enabled;
    int16_t discriminator;


    //std::unordered_map<std::string, c_inject> m_cbmap;

    // Gateway URL for the Discord Websocket
    std::string gateway_url;

    // Websocket++ object
    websocket websocket_o;

    work_ptr rest_work;

    // Bot's token
    std::string token;

    bot_status status;

    ratelimiter ratelimit_o;

    member * _self = nullptr;

    AEGIS_DECL void on_message(websocketpp::connection_hdl hdl, message_ptr msg, shard * _shard);
    AEGIS_DECL void on_connect(websocketpp::connection_hdl hdl, shard * _shard);
    AEGIS_DECL void on_close(websocketpp::connection_hdl hdl, shard * _shard);
    AEGIS_DECL void process_ready(const json & d, shard * _shard);
    AEGIS_DECL void keep_alive(const asio::error_code& error, const int ms, shard * _shard);
};

}

#if defined(AEGIS_HEADER_ONLY)
# include "aegis/aegis.cpp"
#endif // defined(AEGIS_HEADER_ONLY)

