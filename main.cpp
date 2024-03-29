#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>

#include "scanner/DocumentScanner.h"
#include "scanner/ScannedDocument.h"
#include "scanner/SaneScanner.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

ScannedDocument doc;
auto saneScanner = std::make_unique<SaneScanner>();
DocumentScanner scanner = DocumentScanner(std::move(saneScanner));
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
    const char *device_name = "pixma:04A91908";

    ScannedPage page = scanner.scan(device_name, dpi);
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

int main(int argc, char *argv[])
{
    utility::string_t port = U("8080"); // Default port

    if (argc == 2) {
        port = utility::conversions::to_string_t(argv[1]);
    } else if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
        return 1;
    }

    utility::string_t address = U("http://0.0.0.0:");
    address.append(port);

    http::experimental::listener::http_listener listener(address);

    listener.support([](http_request request)
    {
        ucout << request.to_string() << std::endl;
        auto path = request.relative_uri().path();
        if (path == "/documents" && request.method() == methods::POST)
            handle_create_document(request);
        else if (path == "/documents/pages")
            handle_add_page(request);
        else if (path == "/documents" && request.method() == methods::GET)
            handle_get_pdf_document(request);
        else
            request.reply(status_codes::NotFound);
    });

    listener.open().wait();

    ucout << utility::string_t(U("Listening for requests at: ")) << listener.uri().to_string() << std::endl;

    while (true);
}
