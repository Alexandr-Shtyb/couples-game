(() => {

    function createAppTitle(appTitle = 'ПАРОЧКИ') {
        const title = document.createElement('h1');
        title.innerHTML = appTitle;
        title.classList.add('title');
        document.body.append(title);
        return title;
    }

    createAppTitle();

    function createForm() {
        const form = document.createElement('form');
        const input = document.createElement('input');
        const startButton = document.createElement('button');

        document.body.append(form);
        form.append(input);
        form.append(startButton);

        input.placeholder = 'Введите кол-во карточек по' +
            ' вертикали/горизонтали (четное от 2 до 10)';
        startButton.textContent = 'Начать игру';
        input.classList.add('input');
        startButton.classList.add('btn');

        return {
            form,
            input,
            startButton
        }
    }

    const formElements = createForm();

    function getScale() {
        let sizeValueInput = formElements.form.querySelector('input').value;
        if ((sizeValueInput < 2) || (sizeValueInput > 10) || (sizeValueInput % 2 !== 0)) {
            sizeValueInput = '4';
        }

        return sizeValueInput;
    }

    let flag = false;

    formElements.form.addEventListener('submit', (e) => {
        e.preventDefault();

        if (!flag) {
            startProgressEndOfGame();
            flag = true;
        }

        formElements.form.querySelector('input').value = '';
    });

    let timerId;
    let arrayClicks = [];
    let firstCard;
    let selectedCards = 0;
    let checkEnd = false;

    function startProgressEndOfGame(form) {
        const cardsCount = getScale(form);
        const listCards = document.createElement('ul');
        listCards.classList.add('list-cards');

        switch (cardsCount) {
            case '2':
                listCards.classList.add('list-cards2');
                break;
            case '6':
                listCards.classList.add('list-cards6');
                break;
            case '8':
                listCards.classList.add('list-cards8');
                break;
            case '10':
                listCards.classList.add('list-cards10');
                break;
        }

        formElements.form.append(listCards);

        let resultArray = createArray(cardsCount);

        for (let i = 0; i < cardsCount ** 2; i++) {
            const elemOfList = document.createElement('li');
            const buttonLi = document.createElement('button');

            buttonLi.setAttribute('id', 'card-btn');
            buttonLi.classList.add('button-card');
            buttonLi.textContent = i;
            arrayClicks.push(resultArray[buttonLi.textContent]);
            buttonLi.textContent = resultArray[buttonLi.textContent];

            listCards.append(elemOfList);
            elemOfList.append(buttonLi);

            buttonLi.addEventListener('click', (e) => {
                let lastCard = e.target;
                lastCard.classList.add('selected-button-card');

                if ((firstCard !== undefined) && (firstCard !== lastCard)) {
                    if ((parseInt(firstCard.textContent)) !== (parseInt(lastCard.textContent))
                        || (firstCard === lastCard)) {
                        setTimeout(function () {
                            [firstCard, lastCard].forEach(el => el.classList.remove('selected-button-card'));
                            firstCard = undefined;
                        }, 300);
                    } else {
                        [firstCard, lastCard].forEach(el => el.classList.add('selected-button-card'));
                        firstCard.setAttribute('disabled', 'disabled');
                        lastCard.setAttribute('disabled', 'disabled');
                        selectedCards += 2;
                        firstCard = undefined;
                    }
                } else if (firstCard !== lastCard) {
                    firstCard = e.target;
                }

                if (selectedCards === cardsCount ** 2) {
                    if (checkEnd === false) {
                        setTimeout(function () {
                            const question = confirm('Сыграть еще?');

                            if (!question) {
                                window.close();
                            }
                            checkEnd = true;
                            window.location.reload();
                        }, 400);
                    }
                }
            });
        }

        timerId = setTimeout(() => {
            if (checkEnd === false) {
                const question = confirm('Время игры закончилось. Сыграть еще?');

                if (!question) {
                    window.close();
                }
            }
            window.location.reload();
        }, 60000);
        return listCards;
    }

    function doubleAddingToArray(arrayCards, numberCards) {
        arrayCards.push(numberCards);
        arrayCards.push(numberCards);
    }

    function algoFisherYates(array) {
        for (let i = array.length - 1; i > 0; i--) {
            let j = Math.floor(Math.random() * (i + 1));
            let t = array[i];
            array[i] = array[j];
            array[j] = t;
        }
        return array;
    }

    function createArray(sizeValueInput) {
        let arrayCards = [];
        let maxNumberCards = (sizeValueInput ** 2) / 2;
        let numberCards;

        for (let i = 1; i < maxNumberCards; i++) {
            numberCards = (i % maxNumberCards);
            doubleAddingToArray(arrayCards, numberCards);
            numberCards++;
        }

        doubleAddingToArray(arrayCards, numberCards); // добавляем последнюю
        // цифру

        algoFisherYates(arrayCards);

        return arrayCards;
    }
})();

