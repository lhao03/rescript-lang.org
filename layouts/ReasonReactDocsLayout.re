%raw
"require('../styles/main.css')";

%raw
{|
let hljs = require('highlight.js/lib/highlight');
let javascriptHighlightJs = require('highlight.js/lib/languages/javascript');
let ocamlHighlightJs = require('highlight.js/lib/languages/ocaml');
let reasonHighlightJs = require('reason-highlightjs');
let bashHighlightJs = require('highlight.js/lib/languages/bash');
let jsonHighlightJs = require('highlight.js/lib/languages/json');
hljs.registerLanguage('reason', reasonHighlightJs);
hljs.registerLanguage('javascript', javascriptHighlightJs);
hljs.registerLanguage('ocaml', ocamlHighlightJs);
hljs.registerLanguage('sh', bashHighlightJs);
hljs.registerLanguage('json', jsonHighlightJs);
|};

module Link = Next.Link;

// Structure defined by `scripts/extract-tocs.js`
let tocData:
  Js.Dict.t({
    .
    "title": string,
    "headers": array(string),
  }) = [%raw
  "require('../index_data/reason_compiler_toc.json')"
];

module UrlPath = DocsLayout.UrlPath;
module NavItem = DocsLayout.NavItem;
module Category = DocsLayout.Category;
module Toc = DocsLayout.Toc;

let overviewNavs = [|
  NavItem.{name: "Introduction", href: "/docs/reason-react/latest"},
  {name: "Installation", href: "/docs/reason-react/latest/installation"},
  {name: "Intro Example", href: "/docs/reason-react/latest/intro-example"},
|];

let coreNavs = [|
  NavItem.{name: "Components", href: "/docs/reason-react/latest/components"},
  {name: "JSX (Version 3)", href: "/docs/reason-react/latest/jsx"},
  {name: "Event", href: "/docs/reason-react/latest/event"},
  {name: "Style", href: "/docs/reason-react/latest/style"},
  {name: "Router", href: "/docs/reason-react/latest/router"},
  {name: "Working with DOM", href: "/docs/reason-react/latest/dom"},
  {name: "Refs in React", href: "/docs/reason-react/latest/refs"},
|];

let idiomNavs = [|
  NavItem.{name: "Invalid Prop Name", href: "/docs/reason-react/latest/invalid-prop-name"},
  {name: "Props Spread", href: "/docs/reason-react/latest/props-spread"},
  {name: "Component as Prop", href: "/docs/reason-react/latest/component-as-prop"},
  {name: "Ternary Shortcut", href: "/docs/reason-react/latest/ternary-shortcut"},
  {name: "Context & Mixins", href: "/docs/reason-react/latest/context-mixins"},
  {name: "Custom Class / Component Property", href: "/docs/reason-react/latest/custom-class-component-property"},

|];

let recordApiNavs = [|
  NavItem.{name: "JSX (Old, v2)", href: "/docs/reason-react/latest/jsx-2"},
  {name: "Creation, Props & Self", href: "/docs/reason-react/latest/creation-props-self"},
  {name: "Render", href: "/docs/reason-react/latest/render"},
  {name: "Callback Handlers", href: "/docs/reason-react/latest/callback-handlers"},
  {name: "State, Action & Reducer", href: "/docs/reason-react/latest/state-actions-reducer"},
  {name: "Lifecycles", href: "/docs/reason-react/latest/lifecycles"},
  {name: "Instance Variables", href: "/docs/reason-react/latest/instance-variables"},
  {name: "React Ref", href: "/docs/reason-react/latest/react-ref"},
  {name: "Talk to Existing ReactJS Code", href: "/docs/reason-react/latest/interop"},
  {name: "cloneElement", href: "/docs/reason-react/latest/clone-element"},
  {name: "Children", href: "/docs/reason-react/latest/children"},
  {name: "Subscriptions Helper", href: "/docs/reason-react/latest/subscriptions-helper"},
  {name: "Router", href: "/docs/reason-react/latest/router-2"},
|];

let miscNavs = [|
  NavItem.{name: "FAQ", href: "/docs/reason-react/latest/faq"},
|];

let categories = [|
  Category.{name: "Getting Started", items: overviewNavs},
  {name: "Core", items: coreNavs},
  {name: "ReactJS Idiom Equivalents", items: idiomNavs},
  {name: "Record API (deprecated)", items: recordApiNavs},
  {name: "Miscellaneous", items: miscNavs},
|];

[@react.component]
let make = (~components=Markdown.default, ~children) => {
  let router = Next.Router.useRouter();
  let route = router##route;

  let activeToc: option(Toc.t) =
    Belt.Option.(
      Js.Dict.get(tocData, route)
      ->map(data => {
          let title = data##title;
          let entries =
            Belt.Array.map(data##headers, header =>
              {Toc.header, href: "#" ++ header}
            );
          {Toc.title, entries};
        })
    );

  let urlPath = UrlPath.parse(~base="/docs/reason-react", route);

  let breadcrumbs =
    Belt.Option.map(
      urlPath,
      v => {
        let {UrlPath.version} = v;
        let prefix =
          UrlPath.[
            {name: "Docs", href: "/docs"},
            {
              name: "ReasonReact",
              href: "/docs/reason-react/" ++ version,
            },
          ];
        UrlPath.toBreadCrumbs(~prefix, v);
      },
    );

  let title = "ReasonReact";
  let version = "v0.7";

  <DocsLayout
    theme=`Js components categories version title ?activeToc breadcrumbs>
    children
  </DocsLayout>;
};
