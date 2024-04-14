import requests
import os
import re
from bs4 import BeautifulSoup

def download_file(url, directory, cookies):
    response = requests.get(url, cookies=cookies)
    filename = os.path.join(directory, url.split("/")[-1])
    with open(filename, 'wb') as f:
        f.write(response.content)

def main(html_file, cookies):
    with open(html_file, 'r') as f:
        soup = BeautifulSoup(f, 'html.parser')

    tables = soup.find_all('table')
    for table in tables:
        title = table.find('h2').text.strip()
        directory = re.sub(r'[^\w\s]', '', title)
        os.makedirs(directory, exist_ok=True)

        links = table.find_all('a', href=True)
        for link in links:
            url = link['href']
            download_file(url, directory, cookies)
            print(f"Downloaded {url} to {directory}")

if __name__ == "__main__":
    # grades site uses a temp cookie in the href link of the file.
    # if the cookie is valid then will be downloaded
    # to make sure it is valid:
    # sign in manually
    # go to the page of exams
    # copy the related element (that has all the links in it
    # then paste that into me.html
    # let the magic begin

    #maybe no need for this
    cookies = {
        'LastGradesCookie': 'jikogkYlwH7GA8mIp9vvbskdq5DuJQ',
        'LastGradesLogin': 'abdallaha'
        # Add more cookies as needed
    }
    main("me.html", cookies)
