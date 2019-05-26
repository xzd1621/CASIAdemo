#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <boost/algorithm/string/replace.hpp>
#include <string.h>
 
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
 
using namespace std;
 
 

static void print_results(json::value const & value)

{

    if(!value.is_null()) {
        auto userId = value.at(U("userId")).as_integer();
        auto id = value.at(U("id")).as_integer();
        auto title = value.at(U("title")).as_string();
        auto body = boost::algorithm::replace_all_copy(value.at(U("body")).as_string(), "\n", "\\n");
        cout << "Post {userId = " << userId
            << ", id = " << id
            << ", title = \"" << title
            << "\", body = \"" << body
            << "\"}" << endl;
    }
}

static void my_print_results(json::value const & value)
{
    if(!value.is_null()) {
         auto array = value.as_array();
         for(int i=0;i<array.size();i++){
            auto userId = array[i].at(U("userId")).as_integer();
            auto id = array[i].at(U("id")).as_integer();
            auto title = array[i].at(U("title")).as_string();
            auto body = boost::algorithm::replace_all_copy(array[i].at(U("body")).as_string(), "\n", "\\n");
            cout << "Post {userId = " << userId
            << ", id = " << id
            << ", title = \"" << title
            << "\", body = \"" << body
            << "\"}" << endl;
         }
    }
}

static utility::string_t Adminlogin(){
    utility::string_t username;
    utility::string_t password;
    cout<<"LogIn"<<endl<<"UserName:";
    cin>>username;
    cout<<endl<<"Password:";
    cin>>password;
    utility::string_t auth_uri="/?unsername=";
    auth_uri+=username;
    auth_uri+="&password=";
    auth_uri+=password;
    return auth_uri;
}

static void json_get( utility::string_t auth_uri)
{
    http_client client(U("https://jsonplaceholder.typicode.com/"));
    utility::string_t redirect = "posts";
    redirect+=auth_uri;
    uri_builder builder(U(redirect));
    client
    // send the HTTP GET request asynchronous
    .request(methods::GET, builder.to_string())
    // continue when the response is available
    .then([](http_response response) -> pplx::task<json::value> {
        // if the status is OK extract the body of the response into a JSON value
        // works only when the content type is application\json
        if(response.status_code() == status_codes::OK) {
    	printf("redirect status ok");    
    	return response.extract_json();
        }
 
        // return an empty JSON value
        return pplx::task_from_result(json::value());

    })
    // continue when the JSON value is available
    .then([](pplx::task<json::value> previousTask) {
        // get the JSON value from the task and display content from it
        try {
            json::value const & v = previousTask.get();
            //print_results(v);
            my_print_results(v);
        } catch (http_exception const & e) {
            printf("Error exception:%s\n", e.what());
        }
    })
    .wait();
}

static void json_get()
{
    http_client client(U("https://jsonplaceholder.typicode.com/"));
    // Build request URI and start the request.
    //uri_builder builder(U("?format=json"));
 
    //http_client client(U("http://192.168.1.148:8000/"));
    // Build request URI and start the request.
    uri_builder builder(U("posts"));


    client
    // send the HTTP GET request asynchronous
    .request(methods::GET, builder.to_string())
    // continue when the response is available
    .then([](http_response response) -> pplx::task<json::value> {
        // if the status is OK extract the body of the response into a JSON value
        // works only when the content type is application\json
        if(response.status_code() == status_codes::OK) {
    	printf("status ok");    
    	return response.extract_json();
        }
 
        // return an empty JSON value
        return pplx::task_from_result(json::value());

    })
    // continue when the JSON value is available
    .then([](pplx::task<json::value> previousTask) {
        // get the JSON value from the task and display content from it
        try {
            json::value const & v = previousTask.get();
            //print_results(v);
            my_print_results(v);
        } catch (http_exception const & e) {
            printf("Error exception:%s\n", e.what());
            json_get(Adminlogin());
        }
    })
    .wait();
}
 
static void json_post()
{
    http_client client(U("https://jsonplaceholder.typicode.com/"));
    
    json::value json_v ;
    json_v["userId"] = json::value::number(101);
    json_v["title"] = json::value::string("test title");
    json_v["body"] = json::value::string("test body");
 
    client
    .request(methods::POST, U("posts"), json_v)
    .then([](http_response response) -> pplx::task<string_t> {
        if(response.status_code() == status_codes::Created) {
            return response.extract_string();
        }
        return pplx::task_from_result(string_t());
    })
    .then([](pplx::task<string_t> previousTask) {
        try {
            string_t const & v = previousTask.get();
            cout << v << endl;
        } catch (http_exception const & e) {
            printf("Error exception:%s\n", e.what());
        }
    })
    .wait();
}
 
static void json_update()
{
    http_client client(U("https://jsonplaceholder.typicode.com/"));
    
    json::value json_v ;
    json_v["userId"] = json::value::number(101);
    json_v["title"] = json::value::string("test title");
    json_v["body"] = json::value::string("test body");
    
    client
    .request(methods::PUT, U("posts/1"), json_v)
    .then([](http_response response) -> pplx::task<string_t> {
        if(response.status_code() == status_codes::OK) {
            return response.extract_string();
        }
        return pplx::task_from_result(string_t());
    })
    .then([](pplx::task<string_t> previousTask) {
        try {
            string_t const & v = previousTask.get();
            cout << v << endl;
        } catch (http_exception const & e) {
            printf("Error exception:%s\n", e.what());
        }
    })
    .wait();
}
 
static void json_delete()
{
    http_client client(U("https://jsonplaceholder.typicode.com/"));
    
    client
    .request(methods::DEL, U("posts"))
    .then([](http_response response) -> pplx::task<string_t> {
        if(response.status_code() == status_codes::OK) {
            return response.extract_string();
        }
        return pplx::task_from_result(string_t());
    })
    .then([](pplx::task<string_t> previousTask) {
        try {
            string_t const & v = previousTask.get();
            cout << v << endl;
        } catch (http_exception const & e) {
            printf("Error exception:%s\n", e.what());
        }
    })
    .wait();
}

//测试
static void TestRequest()
{
    auto fileStream = std::make_shared<concurrency::streams::ostream>();
    concurrency::streams::ostream outFile = concurrency::streams::fstream::open_ostream(U("result.html")).get();
    *fileStream = outFile;

    http_client client("http://127.0.0.1:8000/api/publisher");
    uri_builder builder("/1/?format=json");
   // builder.append_query(L"q", L"Casablanca CodePlex");

    http_response response = client.request(methods::GET, builder.to_string()).get();
    
    response.body().read_to_end(fileStream->streambuf()).get();
    fileStream->close().get();
}

int main(int argc, char* argv[])
{
    // TestRequest();

    json_get();
    // json_post();
    //json_update();
    //json_delete();
 
    return 0;
}