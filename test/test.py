import requests

BASE_URL = "http://localhost:8085"

def create_document(dpi_value):
    requests.post(f"{BASE_URL}/documents", params={"dpi": dpi_value})
    requests.post(f"{BASE_URL}/documents/pages")
    response = requests.get(f"{BASE_URL}/documents")
    
    assert response.status_code == 200
    assert response.headers.get('content-type') == 'application/pdf'
    # Ensure that the response indeed contains a PDF. This can be done by checking the first few bytes or file signature, for example:
    assert response.content[:4] == b"%PDF"
    # Save to a file if needed
    with open('document.pdf', 'wb') as f:
        f.write(response.content)
