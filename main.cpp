#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include "DocumentScanner.h"
#include "ScannedDocument.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

DocumentScanner scanner;
ScannedDocument doc;
int dpi = 150;

void handle_create_document(http_request request)
{
    auto query_params = web::uri::split_query(request.request_uri().query());
    dpi = stoi(query_params[U("dpi")]);
    doc = ScannedDocument();
    // Your code for creating a document here, using the dpi value
    request.reply(status_codes::OK);
}

void handle_add_page(http_request request)
{
    // Your code for adding a page here
    const char *device_name = "pixma:04A91908"; // Replace this with your actual device name
    int doc_width_mm = 210; // Document width in mm (A4 paper)
    int doc_height_mm = 297; // Document height in mm (A4 paper)
    ScannedPage page = scanner.scan(device_name, dpi, doc_width_mm, doc_height_mm);
    doc.addPage(page);
    request.reply(status_codes::OK);
}

void handle_get_pdf_document(http_request request)
{
    // Your code for getting the PDF document here
    std::vector<uchar> pdfData = doc.getPdfData();
    concurrency::streams::container_buffer<std::vector<uchar>> buffer(std::move(pdfData));
    auto stream = buffer.create_istream();
    request.reply(status_codes::OK, stream, "application/pdf").wait();
}

int main()
{
    http::experimental::listener::http_listener listener(U("http://0.0.0.0:8080"));
    
    listener.support([](http_request request)
    {
        auto path = request.relative_uri().path();
        if (path == "/create_document")
            handle_create_document(request);
        else if (path == "/add_page")
            handle_add_page(request);
        else if (path == "/get_pdf_document")
            handle_get_pdf_document(request);
        else
            request.reply(status_codes::NotFound);
    });

    listener.open().wait();

    ucout << utility::string_t(U("Listening for requests at: ")) << listener.uri().to_string() << std::endl;

    while (true);
}
